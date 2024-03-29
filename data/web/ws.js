var cpjs = cpjs | {};

cpjs = {
    ws: {}
};

cpjs.ws.Client = function(options) {

    this.socket     = null;
    this.cmdQueue   = [];
    this.pendingCmd = null;
    this.onEvent    = null;

    this._sendCmdFromQueue = function() {
        var msg = "";

        if (0 < this.cmdQueue.length) {
            this.pendingCmd = this.cmdQueue.shift();

            msg = this.pendingCmd.name;

            if (null !== this.pendingCmd.par) {
                msg += ";" + this.pendingCmd.par;
            }

            console.info("Websocket command: " + msg);
            this.socket.send(msg);
        }
    };

    this._sendCmd = function(cmd) {

        this.cmdQueue.push(cmd);

        if (null === this.pendingCmd) {
            this._sendCmdFromQueue();
        }
    };

    this._sendEvt = function(evt) {
        if (null !== this.onEvent) {
            this.onEvent(evt);
        }
    }
};

cpjs.ws.Client.prototype.connect = function(options) {
    return new Promise(function(resolve, reject) {
        var wsUrl;

        if (("string" !== typeof options.protocol) ||
            ("string" !== typeof options.hostname) ||
            ("number" !== typeof options.port) ||
            ("string" !== typeof options.endpoint)) {

            reject();
        } else {

            if ("function" === typeof options.onEvent) {
                this.onEvent = options.onEvent;
            }

            try {
                wsUrl = options.protocol + "://" + options.hostname + ":" + options.port + options.endpoint;
                this.socket = new WebSocket(wsUrl);

                this.socket.onopen = function(openEvent) {
                    console.debug("Websocket opened.");
                    resolve(this);
                }.bind(this);

                this.socket.onclose = function(closeEvent) {
                    console.debug("Websocket closed.");
                    if ("function" === typeof options.onClosed) {
                        options.onClosed();
                    }

                    options.evtCallback = null;
                };

                this.socket.onmessage = function(messageEvent) {
                    console.debug("Websocket message: " + messageEvent.data);
                    this._onMessage(messageEvent.data);
                }.bind(this);

            } catch (exception) {
                console.error(exception);
                options.evtCallback = null;
                reject();
            }
        }
    }.bind(this));
};

cpjs.ws.Client.prototype._onMessage = function(msg) {
    var data    = msg.split(";");
    var status  = data.shift();
    var rsp     = {};
    var index   = 0;

    if ("EVT" === status) {
        if (0 < data.length) {
            rsp.event = data[0];

            if ("STARTED" == rsp.event) {
                /* No further parameter */
            } else if ("FINISHED" == rsp.event) {
                rsp.duration = parseInt(data[1]);
                rsp.activeGroup = parseInt(data[2]);
            } else if("TABLE" == rsp.event){
                rsp.activeGroup = parseInt(data[1]);
                rsp.duration = parseInt(data[2]);
                rsp.name = data[3];
            } else {
                console.error("Unknown event: " + rsp.event);
            }
        } else {
            console.error("Unknown empty event.");
        }

        this._sendEvt(rsp);
    } else {
        if (null === this.pendingCmd) {
            console.error("No pending command, but response received.");
        } else if ("ACK" === status) {
            if ("RELEASE" === this.pendingCmd.name) {
                rsp.data = [];
                for(index = 0; index < data.length; ++index) {
                    rsp.data.push(parseInt(data[index], 16));
                }
                this.pendingCmd.resolve(rsp);
            } else if ("GET_GROUPS" === this.pendingCmd.name) {
                rsp.groups = parseInt(data[1]);
                this.pendingCmd.resolve(rsp);
            } else if ("SET_GROUPS" === this.pendingCmd.name) {
                this.pendingCmd.resolve(rsp);
            } else if ("GET_TABLE" === this.pendingCmd.name) {
                rsp.groups = parseInt(data[1]);
                this.pendingCmd.resolve(rsp);
            } else if ("CLEAR" === this.pendingCmd.name) {
                rsp.cleared = parseInt(data[1]);
                this.pendingCmd.resolve(rsp);
            } else if ("SET_NAME" === this.pendingCmd.name) {
                rsp.group = parseInt(data[1]);
                rsp.name = data[2];
                this.pendingCmd.resolve(rsp);
            } else if ("GET_NAME" === this.pendingCmd.name) {
                rsp.group = parseInt(data[1]);
                rsp.name = data[2];
                this.pendingCmd.resolve(rsp);
            } else if ("CLEAR_NAME" === this.pendingCmd.name) {
                rsp.group = parseInt(data[1]);
                this.pendingCmd.resolve(rsp);
            } else if ("REJECT_RUN" === this.pendingCmd.name) {
                this.pendingCmd.resolve(rsp);
            } else {
                console.error("Unknown command: " + this.pendingCmd.name);
                this.pendingCmd.reject();
            }
        } else {
            console.error("Command " + this.pendingCmd.name + " failed.");
            this.pendingCmd.reject();
        }

        this.pendingCmd = null;
    }

    this._sendCmdFromQueue();

    return;
};

cpjs.ws.Client.prototype.release = function(group) {
    return new Promise(function(resolve, reject) {
        if ((null === this.socket) || (typeof(group) === undefined)) {
            reject();
        } else {
            this._sendCmd({
                name: "RELEASE",
                par: group,
                resolve: resolve,
                reject: reject
            });
        }
    }.bind(this));
};

cpjs.ws.Client.prototype.getGroups = function() {
    return new Promise(function(resolve, reject) {
        if (null === this.socket) {
            reject();
        } else {
            this._sendCmd({
                name: "GET_GROUPS",
                par: null,
                resolve: resolve,
                reject: reject
            });
        }
    }.bind(this));
};

cpjs.ws.Client.prototype.setGroups = function(numberOfGroups) {
    return new Promise(function(resolve, reject) {
        if (null === this.socket) {
            reject();
        } else if ((typeof numberOfGroups === 'number') && (isFinite(numberOfGroups))) {
            this._sendCmd({
                name: "SET_GROUPS",
                par: numberOfGroups,
                resolve: resolve,
                reject: reject
            });
        } else {
            reject();
        }
    }.bind(this));
};

cpjs.ws.Client.prototype.getTable = function() {
    return new Promise(function(resolve, reject) {
        if (null === this.socket) {
            reject();
        } else {
            this._sendCmd({
                name: "GET_TABLE",
                par: null,
                resolve: resolve,
                reject: reject
            });
        }
    }.bind(this));
};

cpjs.ws.Client.prototype.clearGroup = function(group) {
    return new Promise(function(resolve, reject) {
        if (null === this.socket){
            reject();
        } else if ((typeof group === 'number') && (isFinite(group))) {
            this._sendCmd({
                name: "CLEAR",
                par: group,
                resolve: resolve,
                reject: reject
            });
        } else {
            reject();
        }
    }.bind(this));
};

cpjs.ws.Client.prototype.setName =  function (group, groupName) {
    return new Promise( function (resolve, reject) {
        if (null === this.socket){
            reject();
        } else if ((typeof group === 'number') && (isFinite(group))) {
            this._sendCmd({
                name: "SET_NAME",
                par: group + ":" + groupName,
                resolve: resolve,
                reject: reject
            });
        } else {
            reject();
        }
    }.bind(this));
};

cpjs.ws.Client.prototype.getName =  function (group) {
    return new Promise( function (resolve, reject) {
        if (null === this.socket){
            reject();
        } else if ((typeof group === 'number') && (isFinite(group))) {
            this._sendCmd({
                name: "GET_NAME",
                par: group,
                resolve: resolve,
                reject: reject
            });
        } else {
            reject();
        }
    }.bind(this));
};

cpjs.ws.Client.prototype.rejectRun =  function () {
    return new Promise( function (resolve, reject) {
        if (null === this.socket){
            reject();
        } else {
            this._sendCmd({
                name: "REJECT_RUN",
                par: null,
                resolve: resolve,
                reject: reject
            });
        }
    }.bind(this));
};