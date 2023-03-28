#!/bin/sh
# PM, le 21/3/21
# PM, le 14/3/22 option -s
# PM, le 24/3/22 correction tests

if [ \( "a$1" = 'a-s' \) ] ; then
	echo "préparation de l'archive à soumettre"
	rep=`whoami|cut -d' ' -f 1`-tpShell
	rm -rf "$rep"
	mkdir "$rep"
	for f in F1 F2 F3 F8 F11 S2 S4 S6 S7 
	do 
		if [ ! \( -f $f \) ] ; then 
			echo "note : fichier $f non trouvé"
		else
			if [ ! \( -x $f \) ] ; then 
				echo "note : le fichier $f ne sera pas ajouté à l'archive, car il n'est"\
				 "pas exécutable"
			else
				cp $f "$rep"
			fi
		fi
	done
	touch "$rep/s"
	tar -cf "$rep".tar "$rep"
	rm -r "$rep"
	echo "prêt : vous pouvez déposer l'archive $rep.tar sous Moodle"
	exit 0
fi

export PATH=.:$PATH
rep=`pwd`

cd $rep
res=KO
if [ \( -f "F1" \) -a \( "`F1 | grep a | tr '[:print:]\n' 'n'`" = "`cat a/hurz`" \) ] ; then
	res=OK
fi
echo "F1 : "$res


cd $rep
res=KO
if [ \( -f "F2" \) -a \( "`F2`" -eq "`cat a/pfurz`" \) ] ; then
	res=OK
fi
echo "F2 : "$res

cd $rep
res=KO
if [  \( -f "F3" \) -a \( -f "a/hurz" \) -a \( -f "a/pfurz" \) ] ; then 
	cp a/hurz a/core
	cp a/pfurz core
	touch a/b/core
	F3
	if [ "`a/bar o`" -eq "`cat foo`" ] ; then
		res=OK
	fi
fi
echo "F3 : "$res

cd $rep
res=KO
if [ \( -f "F8" \) -a \( "`F8 Notice.md`" -eq 1115 \) ] ; then
	res=OK
fi
echo "F8 : "$res

cd $rep
res=KO
if [ \( -f "F11" \) -a \( "`F11 Notice.md`" -le 100 \) -a \( "`F11 Notice.md`" -gt 80 \) ] ; then
	res=OK
fi
echo "F11 : "$res

cd $rep
res=KO
if [ \( -f "S2" \) -a \( \( "`S2 < a/s | wc -l `" -eq 5 \) -o  \( "`S2 < a/s | wc -l `" -eq 6 \) \) ] ; then
	res=OK
fi
echo "S2 : "$res

cd $rep
res=KO
if [ -f "S4" -a \( "`S4 a | wc -l `" -eq 9 \) ] ; then
	res=OK
fi
echo "S4 : "$res
echo "Note : pour ce test (S4), OK et KO ne sont qu'indicatifs"

cd $rep
res=KO
if [ -f "S6"  ] ; then 
	S6 echo -v -o toto -T 14 -S -I -e -t > test
	if [ "`cat test`"  =  '-v -o toto -S -I -x' ] ; then 
	   res=OK
	fi
	rm test
fi
echo "S6 : "$res

cd $rep
res=KO
if [ \( -f "S7" \) -a \( "`S7 esope`" = epose \) ] ; then
	res=OK
fi
echo "S7 : "$res

echo "fini"
