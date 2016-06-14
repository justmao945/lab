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
"set relativenumber

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
" netrw
" =====================================

" Tree view
let g:netrw_liststyle=3

"
let g:netrw_keepdir = 0


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
"set expandtab
set shiftwidth=2 softtabstop=2 tabstop=2

" Allow the backspace key to delete anything in insert mode
set backspace=indent,eol,start

" Don't break words
set linebreak

" Display as many lines as possible
set display=lastline

" Always keep n lines above and below the cursor if possible
set scrolloff=2

" Set auto indent
set autoindent

" Set smart indent
set smartindent

" Set fold method to markers. e.g. {{{ }}}
set foldmethod=marker

" switch case indent style
"set cinoptions=l1

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
set completeopt=menuone,preview,longest

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
let NERDTreeIgnore = ['^a.out$','\.o$','\.obj$','\.pyc$','\.so$','\.dll$', '\.exe$', '\.a$', '\.gcda$', '\.gcno$']

" default is 31
let NERDTreeWinSize = 25


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
" Syntastic settings.
" =====================================
"set statusline+=%#warningmsg#
"set statusline+=%{SyntasticStatuslineFlag()}
"set statusline+=%*

let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0

" =====================================
" vim-go settings.
" =====================================
" default is disabled, let's start the colorful world
let g:go_highlight_functions = 1
let g:go_highlight_methods = 1
let g:go_highlight_structs = 1
let g:go_highlight_operators = 1
let g:go_highlight_build_constraints = 1

" defualt is gofmt
let g:go_fmt_command = "goimports"

" Use this option to show the type info (|:GoInfo|) for the word under the cursor
" automatically. Whenever the cursor changes the type info will be updated.
let g:go_auto_type_info = 1


" go to def with different options
"au FileType go nmap <Leader>ds <Plug>(go-def-split)
au FileType go nmap <Leader>dv <Plug>(go-def-vertical)
au FileType go nmap <Leader>dt <Plug>(go-def-tab)

" open godoc for the word under the cursor
au FileType go nmap <Leader>gd <Plug>(go-doc)
au FileType go nmap <Leader>gv <Plug>(go-doc-vertical)

" open godoc in browser
au FileType go nmap <Leader>gb <Plug>(go-doc-browser)

" show a list of interfaces which is implemented by the type under the type 
au FileType go nmap <Leader>s <Plug>(go-implements)

" show type info for the word under your cursor
au FileType go nmap <Leader>i <Plug>(go-info)

" rename identifier under the cursor to a new name
au FileType go nmap <Leader>e <Plug>(go-rename)


" =====================================
" tagbar settings.
" =====================================
" default has empty lines
let g:tagbar_compact = 1

" default is 40, too width
let g:tagbar_width = 25

" default use big arrows
let g:tagbar_iconchars = ['▸', '▾']


" =====================================
" vim-markdown settings.
" =====================================
" don't fold
let g:vim_markdown_folding_disabled=1


" =====================================
" File types and miscs.
" =====================================
filetype plugin indent on

au BufRead,BufNewFile *.md set filetype=markdown
au BufRead,BufNewFile *.sv set filetype=verilog
au BufRead,BufNewFile *.conf set filetype=json

" This autocommand jumps to the last known position in a file
" just after opening it, if the '" mark is set: >
au BufReadPost * if line("'\"") > 0 && line("'\"") <= line("$") |
                     \ exe "normal g'\"" | endif

" Save buffer to file before leave
au BufLeave * if &modifiable && !empty(bufname("%")) | exe "silent update" | endif

" Auto chdir
autocmd BufEnter * silent! lcd %:p:h

" =====================================
" UI settings
" =====================================
" GUI
if has('gui_running')
  set guioptions=aei

endif


" theme
"if has('unix') && !has('gui_running') && !has("mac")
"  set background=dark
"else
"  set background=light
"endif

let g:solarized_termcolors=256
colorscheme solarized

" platform
if has("win32")
  " sh is default
  set guifont=Consolas:h9:cANSI
elseif has("mac") || system("uname -s") =~ "Darwin"
  set shell=bash
  " don't set them in non-gui
  if has('gui_running')
    set linespace=1 lines=999 columns=999
    set guifont=Monaco:h13
  endif
elseif has("unix")
  set shell=bash
endif


" =====================================
" Shortcut
" =====================================
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
  set guitablabel=%N\ %t
  for i in range(1, 9)
    exe "map <D-" . i . "> :call TabPos_ActivateBuffer(" . i . ")<CR>"
  endfor
  exe "map <D-0> :call TabPos_ActivateBuffer(10)<CR>"
endfunc
if has("gui_running")
  autocmd VimEnter * call TabPos_Initialize()
endif

nmap ,r :NERDTreeFind<CR>
nmap ,n :NERDTreeToggle<CR>

nmap <silent> <C-E> :CtrlPFunky<CR>

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
