// ==UserScript==
// @name    Sogou Cloud IME Helper
// @author  justmao945 AT gmail DOT com
// @exclude http://www.cc98.org/*
// @exclude http://*web*.q*.com/*
// @exclude http://*app*.qq.com/*
// @exclude http://*qz*.qq.com/*
// @include http://*/*
// @include https://*/*
// @run_at  document_end
// ==/UserScript==
//
// * Press Ctrl + , to trigger IME.
//
// * Can't work in HTTPS mode ? Add the command line flag
//      --allow-running-insecure-content
//   to prevent Chrome from checking for insecure content.
// 
// * Can't install from local file ? Add the command line flag
//       --enable-easy-off-store-extension-install
//   to prevent Chrome from checking for off stroe extension.
//
// [0] http://pinyin.sogou.com/cloud/?r=pinyin
// [1] http://stackoverflow.com/questions/10485992/hijacking-a-variable-with-a-userscript-for-chrome
// [2] http://www.catswhocode.com/blog/using-keyboard-shortcuts-in-javascript
// [3] http://code.google.com/p/chromium/issues/detail?id=128413

var code = function(){
  var CTRL  = 17;
  var SPACE = 32;
  var COMMA = 188;
  var isCtrl = false;

  var insIME = function(){
    var js = document.createElement('script');
    js.setAttribute('src', 'http://web.pinyin.sogou.com/web_ime/init2.php');
    document.body.appendChild(js);
  }

  var onkeyup = document.onkeyup;
  document.onkeyup = function(e) {
    if(e.which === CTRL)
      isCtrl=false;
    if( onkeyup && typeof(onkeyup) === 'function' )
      onkeyup(e);
  }

  var onkeydown = document.onkeydown;
  document.onkeydown = function(e){
    if(e.which === CTRL){
      isCtrl = true;
    }
    else if(e.which === COMMA && isCtrl) {
      if( typeof(imeInit) === 'undefined' ){
        insIME();
      }else if( ime_close ){
        imeInit();
      }else{
        imeClose();
      }
    }
    if( onkeydown && typeof(onkeydown) === 'function' )
      onkeydown(e);
  }

  // default install IME
  insIME();
}

var script = document.createElement('script');
script.textContent = '(' + code + ')()';
(document.head||document.documentElement).appendChild(script);
script.parentNode.removeChild(script);

