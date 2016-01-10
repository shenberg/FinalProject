/** Grammars always start with a grammar header. This grammar is called
 *  SPCalculator and must match the filename: SPCalculator.g4
 */
grammar SPCalculator;
/**
* Our parse code will reside in the package SP alongside our implementation of a tree.
*/
@header{
	package SP;
}

@parser::members{
	//We define a new method in our praser. This method will return a string
	//Representing our tree in LISP-Style
	public String getLisp(){
		StatContext ctx = (StatContext)stat();
		if(ctx.exception!=null){
			return new String("Invalid Expression!");		
		}else{
			return ctx.tree.getLisp();
		}
	}
}

//Valid statement is either a termination command || an arithmetical expression followed by a semicolon
stat returns [SPTree tree] : e1=TERMINATION SEMICOLON {$tree = new SPTree($e1.text);}
			   | e2=exp SEMICOLON {$tree = $e2.tree;}
 ; 

exp returns [SPTree tree] : //Define an arithmetical expression here

// parser rules start with lowercase letters, lexer rules with uppercase
TERMINATION: '<>';
SEMICOLON: ';';
