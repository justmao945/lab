// ==UserScript==
// @name    Adblocks
// @author  justmao945 AT gmail DOT com
// @include http://*/*
// @include https://*/*
// @run_at  document_end
// @version rev.20130118
// ==/UserScript==
//
// [1] https://developer.mozilla.org/en/window.location
//

// rules to be blocked
var rules =
{
  "www.bilibili.tv" : {
    ids: ["taobaoid"],
    classes: ["ad-b", "ad-b2", "ad-b3", "ad-b4", "ad-e", "ad-e3", "ad-f"],
    func: null, /* function(loopId, loop) will be called on every loop */
    once: null, /* function(init) will be called only once */
  },

  "www.acfun.tv" : {
    ids: [],
    classes: ["ad"],
  },

  "bt.ktxp.com": {
    ids: [],
    classes: ["money"],
  },

  "dict.youdao.com": {
    ids: ["imgAd", "topImgAd", "ads"],
    classes: [],
    once: function(init){
      var f = document.getElementById("f");
      var onsub = f.onsubmit;
      f.onsubmit = function(){
        init();
        if( typeof(onsub) === 'function' ) onsub();
      };
    },
  },
  
  "www.qiushibaike.com": {
    ids: ["box"],
    classes: ["sponsor", "foot"],
  },
  
  "115.com": {
    classes: ["side-ban"],
  },
  //TODO add new rules here
};


// After timeout, script loops will be breaked.
var timeout = 2100;

// Loops will be repeated between every interval.
var interval = 500;

// nth loop
var loopId = 0;


// ------------------------------------------
//  Main proc
// ------------------------------------------
var rule = rules[location.hostname];
if( ! rule ) return;

// Remove node e from DOM
// return ture if e is not null and removed, or false
var rm = function( /* Element */ e ){
  if( e && e.parentNode ){
    console.warn("[Blocked] {URI:'" + e.baseURI + "', id:'" + e.id
                  + "', class:'" + e.className + "'}");
    e.parentNode.removeChild(e);
    return true;
  }
  return false;
}

// loops for blocking
var loop = function(){
  var cnt = 0;
  if( rule.ids && rule.ids.constructor === Array ){
    for( var i=0; i<rule.ids.length; ++i ){
      if( rm( document.getElementById( rule.ids[i] ) ) ){
        cnt++;
      }
    }
  }

  if( rule.classes && rule.classes.constructor === Array ){
    for( var i=0; i<rule.classes.length; ++i ){
      var es = document.getElementsByClassName( rule.classes[i] );
      for( var j=0; j<es.length; ++j ){
        if ( rm( es[j] ) ){
          cnt++;
        }
      }
    }
  }

  if( typeof( rule.func ) === "function" ){
    if ( rule.func(loopId, loop) ){
      cnt++;
    }
  }
  loopId++;
  return cnt;
}

var init = function(){
  // loop first
  loop();

  // init interval
  var interId = setInterval(loop, interval);

  // clear interval after timeout
  // TODO Smarter stopper according to the value returned by loop() ?
  setTimeout(function(){
    clearInterval(interId);
  }, timeout);
}

if( typeof(rule.once) === "function" ){
  rule.once(init);
}

init();
