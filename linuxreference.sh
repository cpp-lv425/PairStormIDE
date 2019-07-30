if [ ! -d "$HOME/Documents/PairStorm" ]; then
	mkdir $HOME/Documents/PairStorm
fi

if [ ! -d "$HOME/Documents/PairStorm/reference" ]; then
	wget  -O $HOME/Documents/PairStorm/cppref "http://upload.cppreference.com/mwiki/images/b/b2/html_book_20190607.zip"
	cd $HOME/Documents/PairStorm/
	unzip cppref
	rm cppref
fi
