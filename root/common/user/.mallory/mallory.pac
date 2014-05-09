// proxy auto-config template

var direct = 'DIRECT';
var http_proxy = 'SOCKS 127.0.0.1:18081; PROXY 127.0.0.1:18087; DIRECT';

var domains =
{
  "appspot.com": true,
  "blogspot.com": true,
  "feedly.com": true,
  "golang.org": true,
  "twitter.com":  true,
  "wordpress.com": true,
};

// host can override domains
var hosts =
{
  "cdn.sstatic.net": true,
  "d3js.org": true,
  "github.global.ssl.fastly.net": true,
};

// regex to match URL
var regex = 
[
  /^http:\/\/.*google.*/,
];


function host2domain(host) {
  var dotpos = host.lastIndexOf(".");
  if (dotpos === -1)
    return host;
  // Find the second last dot
  dotpos = host.lastIndexOf(".", dotpos - 1);
  if (dotpos === -1)
    return host;
  return host.substring(dotpos + 1);
};

function FindProxyForURL(url, host) {
  var q = hosts[host];
  if( q === true ) return http_proxy;
  else if( q === false ) return direct;
 
  q = domains[host2domain(host)]
  if(q === true) return http_proxy;
  else if(q == false) return direct;

  for(var i = 0; i < regex.length; ++i) {
    if(regex[i].test(url)) {
      return http_proxy;
    }
  }
  return direct;
};
