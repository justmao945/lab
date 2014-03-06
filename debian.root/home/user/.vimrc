" References
" [1] http://fingelrest.silent-blade.org/uploads/Main/vimrc.html
"

" =====================================
" Pathogen Setup
" Now can install plugins in ~/.vim/bundle
" =====================================
execute pathogen#infect()


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

" Tab width
set shiftwidth=4
set softtabstop=4
set tabstop=4

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

" How to show and insert possible completions
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

" Auto completion after .
au FileType go inoremap <expr> <buffer> . CodeAutoCompleteDot()

" Format go before save
au FileType go au! BufWritePre <buffer> silent Fmt


" =====================================
" Clang completion
" =====================================
" doesn't tag {} as error in ()
let c_no_curly_error=1

" Add preview window if is C++
au FileType cpp set completeopt+=preview
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
let NERDTreeIgnore = ['^a.out$','\.o$','\.obj$','\.pyc$','\.so$','\.dll$']


" =====================================
" indentLine settings
" =====================================
" indentLine char
let g:indentLine_char = '┆'


" =====================================
" CtrlP settings.
" =====================================
let g:ctrlp_cmd = 'CtrlPBuffer'

let g:ctrlp_working_path_mode = 'ra'

let g:ctrlp_custom_ignore = {
    \ 'dir':    '\v[\/]\.(git|hg|svn)$',
    \ 'file':   '\v\.(exe|so|dll|obj|o|ndb)$',
    \ 'link':   '',
    \ }


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
if(has('gui_running'))
  set guioptions=ai
  set linespace=3
  set lines=45
  set columns=84
  colorscheme default
  set background=light
else
  colorscheme 256-grayvim
  set background=dark
endif

if has("win32")
  set guifont=Consolas:h9:cANSI
else
  set guifont=Consolas\ 12
  set guifontwide=SimSun\ 11
endif

" custom the default theme
  hi Comment guifg=#7f7f7f
  hi LineNr  guifg=#7f7f7f
  " Set color(light grey) of vert split line and status lines.
  hi VertSplit guibg=#7f7f7f guifg=#7f7f7f
  hi StatusLine guifg=#7f7f7f
  hi StatusLineNC guifg=#7f7f7f

" =====================================
" Keyboard
" =====================================

" Tmux will send xterm-style keys when its xterm-keys option is on
if $TERM =~ '^screen'
  exe "set <xUp>=\e[1;*A"
  exe "set <xDown>=\e[1;*B"
  exe "set <xRight>=\e[1;*C"
  exe "set <xLeft>=\e[1;*D"
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

map  <silent> <C-q>  <esc>:qa!<cr>
map! <silent> <C-q>  <esc>:qa!<cr>

map  <silent> <C-S>  <esc>:update<cr>
map! <silent> <C-S>  <esc>:update<cr>

imap  <silent> <M-/>  <C-X><C-P>

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

" vim:set ts=2 sw=2:
