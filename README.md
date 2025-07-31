# get-free-port

Get a random free port synchronously using libuv tcp handle bind, without waiting for a server to listen.

## example

```javascript
let port = getFreePort();
console.log(port);
// log: 45073
```
