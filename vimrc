"use VIM settings
set nocompatible

"list of file encodings to try
"set fileencodings=iso-2022-jp,ucs-bom,utf8,sjis,euc-jp,latin1

"change CWD to the dir of the file in the buffer
":autocmd BufEnter * cd %:p:h
filetype plugin indent on   "autoguess by file extension
set tabstop=4               "number of spaces in a tab, used in expandtab
set softtabstop=4           "number of spaces in a tab, used in smarttab
set shiftwidth=4            "used in shifting or C indenting    
set expandtab               "insert spaces for tabs
set smarttab                "somewhat smarter tab control
set cindent                 "C indenting
set number                  "print line numbers
set incsearch               "incremental search
set hlsearch                "highlight search matches
"set ignorecase              "only for smartcase below
set smartcase               "if no caps, case insensitive
set ruler                   "show the cursor
set wildmenu                "print menu completions
set autowrite               "write buffer to file when switching
set scrolloff=5             "keep lines of context when scrolling
set foldmethod=syntax       "fold according to syntax hl rules
set foldlevel=99            "default to all open
colorscheme slate

"list of places to look for tags
set tags=./tags,tags

"cscope settings
set cst                 "use tags interface
set csto=0              "search cscope first

"global cscope database
if filereadable($HOME."/Code/tags/cscope.out")
    cs add ~/Code/tags/cscope.out
endif

" add any cscope database in current directory
if filereadable("cscope.out")
    cs add cscope.out
endif

" 0 or s: Find this C symbol
" 1 or g: Find this definition
" 2 or d: Find functions called by this function
" 3 or c: Find functions calling this function
" 4 or t: Find this text string
" 6 or e: Find this egrep pattern
" 7 or f: Find this file
" 8 or i: Find files #including this file
nmap <C-\> :cs find s <C-R>=expand("<cword>")<CR><CR>

"set cinwords
"set cinkeys

"add dictionary to ^N completion
"set dictionary+=/usr/share/dict/words
"set complete+=k        

"next/prev buffer
nnoremap <silent> <C-N> :bnext<CR>
nnoremap <silent> <C-P> :bprevious<CR>

"save buffer
noremap <F1> <Esc>:w<CR>
noremap! <F1> <Esc>:w<CR>

"cancel highlighting
nnoremap <C-C> :nohl<CR>

"insert newline
nnoremap <C-J> i<CR><Esc>==

"search+replace word under cursor
nnoremap <C-S> :,$s/\<<C-R><C-W>\>/

"no annoying bell
set t_vb=

" Switch on syntax highlighting if it wasn't on yet.
if !exists("syntax_on")
    syntax on
endif
