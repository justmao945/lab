" References
" [1] http://fingelrest.silent-blade.org/uploads/Main/vimrc.html
"

" =====================================
" Pathogen Setup
" Now can install plugins in ~/.vim/bundle
" =====================================
if filereadable($HOME."/.vim/autoload/pathogen.vim")
  execute pathogen#infect()
endif


" =====================================
" General Setup
" =====================================
" Disable compatibility with vi(must come first)
set nocompatible 

" Restroe screen contents when exiting vim
set restorescreen

" Enable the mouse for all modes
set mouse=a 

" Hide mouse while typing text
set mousehide 

" Preferably use Unix file format
set fileformats=unix,dos

" Display line numbers
set number

" Display commands in the bottom right corner as they are typed
set showcmd

" Size of history
set history=150

" Don't keep backups
set nobackup
set writebackup

" Disable any kind of bell
set t_vb=
set noerrorbells
set novisualbell

" Smoother redraws
set ttyfast

" Do not redraw when running macros
set lazyredraw

" Auto change to current directory. 
" e.g. Nautilus does not change system directories.
set autochdir

" Set fixed swap file dir
set dir=~/.vimswap//,~/.cache//,/tmp//,.

" set default shell to sh
set shell=sh


" =====================================
" Text edition
" =====================================
" Syntax highlighting
syntax on

" Dont't cut lines
set textwidth=0

" Don't wrap lines
set nowrap

" Dont't put two spaces after . ? ! when joining lines
set nojoinspaces

" Insert spaces instead of tabs
set expandtab

" Allow the backspace key to delete anything in insert mode
set backspace=indent,eol,start

" Don't break words
set linebreak

" Display as many lines as possible
set display=lastline

" Always keep n lines above and below the cursor if possible
set scrolloff=5

" Set auto indent
set autoindent

" Set smart indent
set smartindent

" Set fold method to markers. e.g. {{{ }}}
set foldmethod=marker

" =====================================
" Searching/Replacing
" =====================================
" Incremental search
set incsearch

" Highlight search terms
set hlsearch

" Ignore case when searching, but only if all letters are lowercase
set smartcase
"set ignorecase

" Wrap search when EOF is reached
set wrapscan

" Always replace all occurrences on a line, not just the first one
set gdefault


" =====================================
" Insert mode completion
" =====================================
" Don't show possible completions that don't match the case of existing text
set infercase

" Don't show more than n items in the popup menu
set pumheight=8

" General complete option
set completeopt=longest,menuone

" =====================================
" Status line
" =====================================
" Display the status line if more than one window
set laststatus=1

" Show the current editing status
set showmode

" Human readable file size
fu! StatuslineFilesize()
  let filesize = line2byte(line("$") + 1) - 1
  if filesize < 0
    return ""
  elseif filesize < 1024
    return " | " . filesize . " Bytes"
  elseif filesize < 1024*1024
    return " | " . (filesize / 1024) . " KB"
  else
    return " | " . (filesize / (1024*1024)) . " MB"
  endif
endf

" Format string
set statusline=%t\ \|\ %L\ lines%{StatuslineFilesize()}\ %y%m%=%l,%c\ %4p%%\ -


" =====================================
" Auto completion helper
" =====================================
func! CodeAutoCompleteDot()
  for id in synstack(line('.'), col('.') - 1)
    if synIDattr(id, 'name') =~ 'Comment\|String\|Number\|Char\|Label\|Special'
      return '.'
    endif
  endfor
  return ".\<C-x>\<C-o>"
endf

" =====================================
" Python
" =====================================
au FileType python inoremap <expr> <buffer> . CodeAutoCompleteDot()

" =====================================
" Golang
" =====================================
" gofmt with quickfix
"   :ll to see errors
"   :lopen to open the quickfix window
func! GoFmt()
  let cmd = printf("gofmt -w %s", shellescape(expand('%')))
  let raw = split(system(l:cmd), '\n')
  let errors = []
  for line in raw
    let tokens = matchlist(line, '^\(.\{-}\):\(\d\+\):\(\d\+\)\s*\(.*\)')
    if !empty(tokens)
      call add(errors, {"filename": tokens[1],
                       \"lnum":     tokens[2],
                       \"col":      tokens[3],
                       \"text":     tokens[4]})
    endif
  endfor
  if !empty(errors)
    call setloclist(0, errors, 'r')
    ll
  else
    call setloclist(0, [], 'r')
    " I don't know the why ':e' clears syntax...
    "call feedkeys(":e\<Enter>")
  endif
endf

" Auto completion after .
au FileType go inoremap <expr> <buffer> . CodeAutoCompleteDot()

" Format go before save
au BufWritePost *.go call GoFmt()


" =====================================
" Clang completion
" =====================================
" doesn't tag {} as error in ()
let c_no_curly_error=1

set previewheight=4

" Set to debug level 1, only used by plugin developer
let g:clang_debug = 0

" Auto completion is OK for the new async mode
let g:clang_auto = 1

" Add clang options for C sources
let g:clang_c_options = ''

" Add clang options for C++ sources
let g:clang_cpp_options = ''


" =====================================
" NERDTree settings
" =====================================
let NERDTreeIgnore = ['^a.out$','\.o$','\.obj$','\.pyc$','\.so$','\.dll$', '\.exe$', '\.a$', '\.d$', '\.gcda$', '\.gcno$']


" =====================================
" CtrlP settings.
" =====================================
let g:ctrlp_cmd = 'CtrlPMRU'

let g:ctrlp_working_path_mode = 'ra'

let g:ctrlp_custom_ignore = {
    \ 'dir':    '\v[\/]\.(git|hg|svn)$',
    \ 'file':   '\v\.(exe|so|dll|obj|o|ndb|d|gcda|gcna)$',
    \ 'link':   '',
    \ }


" =====================================
" javascript syntax settings.
" =====================================
let g:used_javascript_libs = 'jquery,angularjs'


" =====================================
" File types and miscs.
" =====================================
filetype plugin indent on

au BufRead,BufNewFile *.md set filetype=mkd

" This autocommand jumps to the last known position in a file
" just after opening it, if the '" mark is set: >
au BufReadPost * if line("'\"") > 0 && line("'\"") <= line("$") |
                     \ exe "normal g'\"" | endif

" Save buffer to file before leave
au BufLeave * if &modifiable && !empty(bufname("%")) | exe "silent update" | endif

" Add man page
runtime! ftplugin/man.vim


" =====================================
" UI settings
" =====================================
" !! background should be set after colorscheme
set guioptions=ai
colorscheme hemisu

if has("win32")
  set guifont=Consolas:h9:cANSI
elseif has("mac") || system("uname -s") =~ "Darwin"
  set shiftwidth=2 softtabstop=2 tabstop=2
  set guifont=Monaco:h12
  if has('gui_running')
    set linespace=1 lines=40 columns=90
  endif
elseif has("unix")
  set shiftwidth=4 softtabstop=4 tabstop=4
  set guifont=Consolas\ 12
  set guifontwide=SimSun\ 11
  if has('gui_running')
    set linespace=3 lines=45 columns=84
    set bg=light
  endif
endif


" =====================================
" Shortcut
" =====================================
" Toggle sidebar: NERDTree
fu! ToggleSidebar()
  let b = bufnr("%")
  exe "NERDTreeToggle"
  exe bufwinnr(b) . "wincmd w"
endf

map  <silent> <C-N>  <esc>:call ToggleSidebar()<cr>
map! <silent> <C-N>  <esc>:call ToggleSidebar()<cr>

map  <silent> <C-J>  <esc>:tabprevious<cr>
map! <silent> <C-J>  <esc>:tabprevious<cr>

map  <silent> <C-K> <esc>:tabnext<cr>
map! <silent> <C-K> <esc>:tabnext<cr>

" leader section
map  <silent> <Leader>d <esc>:tabclose<cr>
map  <silent> <Leader>t <esc>:tabnew .<cr>

map  <silent> <Leader>c <esc>:ClangClosePreviewDiagWindow<cr>


" =====================================
" User plugins list
" =====================================

"  Plugins list
"  * vim-pathogen
"     -- https://github.com/tpope/vim-pathogen.git
"  * vim-nerdtree
"     -- https://github.com/scrooloose/nerdtree.git
"  * vim-clang
"     -- https://github.com/justmao945/vim-clang.git
"  * ctrlp.vim
"     -- https://github.com/kien/ctrlp.vim.git
"  * javascript-libraries-syntax.vim
"     -- https://github.com/othree/javascript-libraries-syntax.vim
"
"  Theme list
"  * vim-hemisu
"     -- https://github.com/noahfrederick/vim-hemisu.git
"
" vim:set ts=2 sw=2:
