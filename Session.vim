let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd /mnt/d/NewProjects/NBDiskReader/Trunk
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +6 /mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/inc/Modules/DiskImages/cDiskCore.h
badd +29 /mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/src/Modules/DiskImages/cDiskCore.cpp
argglobal
%argdel
$argadd .
edit /mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/src/Modules/DiskImages/cDiskCore.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
wincmd =
argglobal
balt /mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/inc/Modules/DiskImages/cDiskCore.h
setlocal foldmethod=expr
setlocal foldexpr=v:lua.require'lazyvim.util'.ui.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
let s:l = 29 - ((28 * winheight(0) + 46) / 93)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 29
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("/mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/inc/Modules/DiskImages/cDiskCore.h", ":p")) | buffer /mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/inc/Modules/DiskImages/cDiskCore.h | else | edit /mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/inc/Modules/DiskImages/cDiskCore.h | endif
if &buftype ==# 'terminal'
  silent file /mnt/d/NewProjects/NBDiskReader/Trunk/NBLibrary/inc/Modules/DiskImages/cDiskCore.h
endif
setlocal foldmethod=expr
setlocal foldexpr=v:lua.require'lazyvim.util'.ui.foldexpr()
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
let s:l = 6 - ((5 * winheight(0) + 46) / 93)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 6
normal! 02|
wincmd w
wincmd =
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
