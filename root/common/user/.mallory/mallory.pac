// proxy auto-config template

var direct = 'DIRECT';
var http_proxy = 'SOCKS 127.0.0.1:18081; PROXY 192.9.200.237:18087; DIRECT';

var domains =
{
  "amazon.com": true,
  "angulardart.org": true,
  "angularjs.org": true,
  "appspot.com": true,
  "atom.io": true,
  "blogspot.com": true,
  "blogspot.co.at": true,
  "chromium.org": true,
  "cloudfront.net": true,
  "coco2d-iphone.org": true,
  "coco2d-x.org": true,
  "facebook.net": true,
  "feedly.com": true,
  "golang.org": true,
  "gstatic.com": true,
  "makegameswith.us": true,
  "ng-book.com": true,
  "nodejs.org": true,
  "twitter.com":  true,
  "wordpress.com": true,
  "ytimg.com": true,
  "youtube.com": true,
};

// host can override domains
var hosts =
{
  "api.github.com": true,
  "cdn.bitbucket.org": true,
  "cdn.sstatic.net": true,
  "d3js.org": true,
  "github.global.ssl.fastly.net": true,
};

// regex to match URL
var regexNot = 
[
  /.*calendar\.google\.com.*/,
  /.*talk.*\.google\.com.*/,
];

var regex = 
[
  /.*google.*/,
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

  for(var i = 0; i < regexNot.length; ++i) {
    if(regexNot[i].test(url)) {
      return direct;
    }
  }

  for(var i = 0; i < regex.length; ++i) {
    if(regex[i].test(url)) {
      return http_proxy;
    }
  }
  return direct;
};
