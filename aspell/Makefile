
all: aspell-libmarpa-api

aspell-libmarpa-api:
	cat ../cm_dist/api_docs/libmarpa_api.info | \
	  aspell list --home-dir=. --personal=libmarpa-api-ignore.txt -t | sort | uniq

