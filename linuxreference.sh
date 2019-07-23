MYPWD=${PWD}
if [ ! -d "$PWD/res" ]; then
	mkdir res
fi

if [ ! -d "$PWD/res/reference" ]; then
	wget  -O $MYPWD/res/cppref "http://upload.cppreference.com/mwiki/images/b/b2/html_book_20190607.zip"
	cd $MYPWD/res/
	unzip cppref
	rm cppref
fi
