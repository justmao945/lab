#coding:utf-8

""" FUDAN XUANKE justmao945@gmail.com """

import sys
import urllib
import urllib2
import cookielib
import Image
import cStringIO
import data


def _u(s):
  """
  This function is a stupid one, but it really works!
  convert 's' to utf8
  """
  if not s: return s
  s = s.strip()
  if not s: return s
  for c in ('utf8','gbk','big5','jp','kr'):
    try: return s.decode(c).encode('gbk')
    except: pass
  return s

def init():
  """    Init urllib2 support cookie and Data image   """
  _cookie = urllib2.HTTPCookieProcessor(cookielib.CookieJar())
  urllib2.install_opener(urllib2.build_opener(_cookie, urllib2.HTTPHandler))
  
  _data_image = Image.open('data.bmp')
  if not _data_image:
    sys.stderr.write('Can not open file: data.bmp\n');
    return False
  _data_images = []
  for i in range(0, 80, 8):
    _data_images.append(tuple(_data_image.crop((i,0,i+8,16)).getdata()))
  return _data_images



def get_rand(data_images, img):
  """ 
  Require PIL. 
  image: crop to 4 blocks, position: ( 4,4)->(12,20); (18,4)->(26,20); (32,4)->(40,20); (46,4)->(54,20);
  data_image: crop to 10 blocks, Positon from (0,0) to (80,16)
  """
  _image = Image.open(cStringIO.StringIO(img.read()))
  _black = (0,0,0)
  _res = [-1,-1,-1,-1]
  for i in range(4, 47, 14):
    _image_block = tuple(_image.crop((i,4,i+8,20)).getdata())
    for j, data_image in enumerate(data_images):
      _tmp = -1
      for k in range(0, 8*16):
        if data_image[k] == _black:
          if _tmp < 0:
            _tmp = k
          elif _image_block[k] != _image_block[_tmp]:
            break;
      else: ## find 'j' is the number
        _res[(i-4)/14] = j
    
  return ''.join(str(i) for i in _res)
        

def login(data_images, user, passwd):
  _XK_LOGIN='http://xk.fudan.edu.cn/xk/loginServlet'
  _XK_LOGIN_IMAGE='http://xk.fudan.edu.cn/xk/image.do'

  if not user:
    sys.stderr.write('ERROR:user is empty!\n');
  if not passwd:
    sys.stderr.write('ERROR:passwd is empty!\n');

  _img = urllib2.urlopen(_XK_LOGIN_IMAGE)
  
  _postdata = urllib.urlencode({
    'studentId':user,
    'password':passwd,
    'rand':get_rand(data_images, _img),
    'Submit2':'Submit'
    })
  
  _req = urllib2.Request(url=_XK_LOGIN, data=_postdata)
  _res = urllib2.urlopen(_req).read()
  return _res.find('input.jsp')


def generate_course(data_images, selectionId):
  _XK_SELECT='http://xk.fudan.edu.cn/xk/doSelectServlet'
  _XK_SELECT_IMAGE='http://xk.fudan.edu.cn/xk/image.do'
  _img = urllib2.urlopen(_XK_SELECT_IMAGE)
  
  _postdata = urllib.urlencode({
    'selectionId':selectionId,
    'xklb':'ss',
    'rand':get_rand(data_images, _img),
    })
  _req = urllib2.Request(url=_XK_SELECT, data=_postdata)
  _res = urllib2.urlopen(_req).read(4096)
  return _res


### main
if __name__ == '__main__':
  data_images = init()
  if not data_images:
    sys.exit(1)
  
  sys.stdout.write('Logining the system...Please wait a minute\n')
  if login(data_images, data.USER, data.PASSWD) == -1:
    sys.stderr.write('Can not login the system...\nPlease check your ID and Password in data.py\n')
    sys.exit(1)
  
  cut_msg = lambda msg: _u(msg[msg.find('(\"')+2 : msg.find('\")')].replace('\\n','\n'))
  if type(data.COURSES) == str:
     msg = generate_course(data_images, data.COURSES)
     
     sys.stdout.write('Course: %s --> %s\n' % (data.COURSES, cut_msg(msg)) )
  elif type(data.COURSES) == tuple:
    for c in data.COURSES:
      c = c.strip()
      if c:
        msg = generate_course(data_images, c)
        sys.stdout.write('Course: %s --> %s\n' % (c, cut_msg(msg)) )
  else:
    sys.stderr.write('COURSES format wrong in data.py\n')
    sys.exit(1)

  sys.exit(0)


