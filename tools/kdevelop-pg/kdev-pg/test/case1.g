%token A ("A"), B ("B"), C ("C") ;;
(r2 | 0) r3 -> rr;;
r1 (A r1)* -> r2 ;;
C -> r1 ;;
A -> r3 ;;
