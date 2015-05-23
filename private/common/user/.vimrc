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
" 256 color terminal
set t_Co=256

" share clipboard
"set clipboard+=unnamed

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

" Display relative line number
set relativenumber

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
set shiftwidth=2 softtabstop=2 tabstop=2

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

" switch case indent style
set cinoptions=l1

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
set completeopt=longest,menuone,preview

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
" Clang completion
" =====================================
" doesn't tag {} as error in ()
let c_no_curly_error=1

set previewheight=1

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
" File types and miscs.
" =====================================
filetype plugin indent on

au BufRead,BufNewFile *.md set filetype=mkd
au BufRead,BufNewFile *.sv set filetype=verilog

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
set guioptions=ai

if has('unix') && !has('gui_running') && !has("mac")
  set background=dark
else
  set background=light
endif

if has("win32")
  " sh is default
  set guifont=Consolas:h9:cANSI
elseif has("mac") || system("uname -s") =~ "Darwin"
  set shell=sh
  " don't set them in non-gui
  if has('gui_running')
    set linespace=1 lines=40 columns=84
    set guifont=Monaco:h13
  endif
elseif has("unix")
  set shell=sh
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

" move between windows
map <silent> <C-J> <C-W>j
map <silent> <C-K> <C-W>k
map <silent> <C-H> <C-W>h
map <silent> <C-L> <C-W>l

" tab page switch
func! TabPos_ActivateBuffer(num)
  let s:count = a:num
  exe "tabfirst"
  exe "tabnext" s:count
endfunc

func! TabPos_Initialize()
  for i in range(1, 9)
    exe "map <M-" . i . "> :call TabPos_ActivateBuffer(" . i . ")<CR>"
  endfor
  exe "map <M-0> :call TabPos_ActivateBuffer(10)<CR>"
endfunc
autocmd VimEnter * call TabPos_Initialize()


" =====================================
"  Auto completion
" =====================================
func! ShouldComplete()
  for id in synstack(line('.'), col('.') - 1)
    if synIDattr(id, 'name') =~# 'Comment\|String\|Number\|Char\|Label\|Special'
      return 0
    endif
  endfor
  return 1
endf

func! CompleteDot()
  if ShouldComplete()
    return ".\<C-x>\<C-o>"
  endif
  return '.'
endf

au FileType go,ruby,python imap <expr> <buffer> . CompleteDot()


" =====================================
" User plugins list
" =====================================
" at lab/vim-plugins


" vim:set ts=2 sw=2:
