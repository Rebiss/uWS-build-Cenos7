const uWS = require('./dist/uws.js');
const port = 4000;

const app = uWS.App({
}).get('/*', (res, req) => {
  res.end('Hello World!');
}).listen(port, (token) => {
  if (token) {
    console.log('Listening to port ' + port);
  } else {
    console.log('Failed to listen to port ' + port);
  }
});