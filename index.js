const {getFreePort} = require('bindings')('get_free_port');

module.exports = getFreePort;
