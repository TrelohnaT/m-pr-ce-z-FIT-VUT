NAME=proj4
do: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME)
	latex $(NAME).tex
	latex $(NAME).tex
	dvips $(NAME).dvi
	pdflatex $(NAME).tex