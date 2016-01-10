/** Grammars always start with a grammar header. This grammar is called
 *  SPCalculator and must match the filename: SPCalculator.g4
 */
grammar SPCalculator;

/**
* All generated files must reside in the package SP alongside our implementation
* of the tree and the main function.
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

//Valid statement is either a termination command || an arithmetical expression
stat returns [SPTree tree]		: (e1=TERMINATION {$tree = new SPTree($e1.text);}
								| e2=assign {$tree = $e2.tree; }
								| e3=exp {$tree = $e3.tree;}
								) END
								;

// assignment statement
assign returns [SPTree tree]    : e1=variable o1=EQUALS e2=exp {$tree = new SPTree($o1.text);
																$tree.insertChild($e1.tree);
																$tree.insertChild($e2.tree);}
								;

//Check general exp
exp returns [SPTree tree]       : e1=as_exp {$tree = $e1.tree;}
                                ;

//Check for binary add or sub
as_exp returns [SPTree tree]	: e1=as_exp o1=ADDSUB e2=as_exp {$tree = new SPTree($o1.text);
								        			             $tree.insertChild($e1.tree);
													             $tree.insertChild($e2.tree);}
								| e3=md_exp {$tree = $e3.tree;}
								;

//Check for binary multiply or divide
md_exp returns [SPTree tree]	: e1=md_exp o1=MULDIV e2=md_exp {$tree = new SPTree($o1.text);
																 $tree.insertChild($e1.tree);
																 $tree.insertChild($e2.tree);}
								| e3=do_exp {$tree = $e3.tree;}
								;

//Check for binary dollar operation
do_exp returns [SPTree tree]	: e1=do_exp o1=DOL e2=do_exp {$tree = new SPTree($o1.text);
															  $tree.insertChild($e1.tree);
															  $tree.insertChild($e2.tree);}
								| e3=un_exp {$tree = $e3.tree;}
								;

//Check for unary plus or minus
un_exp returns [SPTree tree] 	: o1=ADDSUB e1=un_exp {$tree = new SPTree($o1.text);
													   $tree.insertChild($e1.tree);}
								| e2=im_exp {$tree = $e2.tree;}
								;

//Check for immediate integer
im_exp returns [SPTree tree]	: e1=INT {$tree = new SPTree($e1.text);}
								| LB e2=exp RB {$tree = $e2.tree;}
								| e3=func_call {$tree = $e3.tree;}
								| e4=variable {$tree = $e4.tree;}
								;

func_call returns [SPTree tree] : ( MIN {$tree = new SPTree("%");}
								  | MAX {$tree = new SPTree("@");}
								  | AVG {$tree = new SPTree("!");}
								  | MEDIAN {$tree = new SPTree("?");}
								  )
								  RB e1=exp {$tree.insertChild($e1.tree);}
								  (COMMA e2=exp {$tree.insertChild($e2.tree);})*
								  LB
								;

variable returns [SPTree tree] : e1=VAR {$tree = new SPTree($e1.text);}
							   ;

// parser rules start with lowercase letters, lexer rules with uppercase
TERMINATION: '<>';
ADDSUB:  '+'|'-';
MULDIV: '*'|'/';
LB:'(';
RB:')';
DOL: '$';
INT: [0-9]+;
WS: [ \t\r\n]+ -> skip;
EQUALS: '=';
MIN: 'min';
MAX: 'max';
AVG: 'average';
MEDIAN: 'median';
COMMA: ',';
VAR: [A-Za-z]+;
END: ';' .*;
