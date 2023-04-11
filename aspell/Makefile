
all: aspell-libmarpa-api

libmarpa-api-ignore.txt: libmarpa-api-ignore.source
	sed -e 's/#.*$$//' -e 's/ *$$//' $? > $@

aspell-libmarpa-api: libmarpa-api-ignore.txt
	cat ../cm_dist/api_docs/libmarpa_api.info | \
	  aspell list --home-dir=. --personal=libmarpa-api-ignore.txt -t | sort | uniq

