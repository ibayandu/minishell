
$> echo deneme > x.txt ismet

--------------------

$> export a="'"

$> echo $a
'
$> echo $a$HOME$a
'/home/yzeybek'

--------------------

>$ echo */*/deneme* && ( echo "Başlangiç: $(date)" && echo << delim) && echo "Alt
işlem başarıli" || echo "Alt işlem başarısız" | grep "içerik" || ech
o "Eşleşme bulunamadı" 123> output.txt 2> deneme.txt && cat < || ech
o "Yeni dosyaya yazılan içerik" | (export a=deneme && echo $a)
bash: syntax error near unexpected token `||'
> fghjkl
> hjkl
> delim

-------------------

$> echo a > ||
bash: syntax error near unexpected token `||'

--------------------

>$ echo a ||
> echo b ||
> echo c
a

---------------------
