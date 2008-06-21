
(**********
 ** something
 *)

class Main inherits IO {  -- a comment
   add(x: Int, y: Int): Int { x + y };
   identity(x: Object): Object { x };

   main(): SELF_TYPE {
	out_string("Hello, World.\n")
   };
};
