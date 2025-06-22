
$> echo deneme > x.txt ismet

--------------------

$> export a="'"

$> echo $a
'
$> echo $a$HOME$a
'/home/yzeybek'

--------------------

$> echo */*/deneme* && ( echo "Başlangiç: $(date)" && echo << delim) && echo "Alt
işlem başarıli" || echo "Alt işlem başarısız" | grep "içerik" || ech
o "Eşleşme bulunamadı" 123> output.txt 2> deneme.txt && cat < a || ech
o "Yeni dosyaya yazılan içerik" | (export a=deneme && echo $a)
bash: syntax error near unexpected token `||'
> fghjkl
> hjkl
> delim

-------------------

$> echo a > ||
bash: syntax error near unexpected token `||'

--------------------

$> echo a ||
> echo b ||
> echo c
a

---------------------

$> cat << (
bash: syntax error near unexpected token `('

---------------------

$> cat << a
>
bash: warning: here-document at line 49 delimited by end-of-file (wanted `a')

---------------------

$> cat << 😄
> dsds
> dsdsfdgf
> 😄
dsds
dsdsfdgf

---------------------

$> cat << abidik'gubiiiii      dik'
> dsadşadöa
> dösalşdaşabidikgubiiiii      dikdsadsa
> abidik'gubiiiii      dik'dlfmelfmes
> abidik'gubiiiii      dik
> abidikgubiiiii      dik
dsadşadöa
dösalşdaşabidikgubiiiii      dikdsadsa
abidik'gubiiiii      dik'dlfmelfmes
abidik'gubiiiii      dik

----------------------

$> cat << dimi"abidik'gubiiiii      dik'"blabla
> dimi"abidik'gubiiiii      dik'blabla
> dimiabidik'gubiiiii      dik'blabla
dimi"abidik'gubiiiii      dik'blabla

-----------------------

$> cat << 1 && cat << 2 && cat << 3 && cat << 4 && cat << 5 && cat << 6 && cat << 7 && cat << 8 && cat << 9 && cat << 10 && cat << 11 && cat << 12 && cat << 13 && cat << 14 && cat << 15 && cat << 16
>...

-----------------------

$> cat << 1 && cat << 2 && cat << 3 && cat << 4 && cat << 5 && cat << 6 && cat << 7 && cat << 8 && cat << 9 && cat << 10 && cat << 11 && cat << 12 && cat << 13 && cat << 14 && cat << 15 && cat << 16 && cat << 17

------------------------

$> echo $
$


