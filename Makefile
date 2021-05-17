test: eval.c eval.h 
	rm -f eval
	gcc eval.c -Wall -g -o eval
	./eval "((0=>1) + (0.1))"
	./eval 1+0.1.0+1
	./eval "(1=>(NON (1+00).1))"
	./eval "(1=>(NON (1+0).1))"
	./eval "(+1+0.NON1.(1=>1)"
	
eval: eval.c eval.h

	gcc -Wall -g -o eval eval.c
	
clean:
	rm -f eval

