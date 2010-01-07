/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2009 Shannon Weyrick <weyrick@roadsend.com>
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****
*/

#include "rphp/analysis/pPassManager.h"
#include "rphp/analysis/pPass.h"
#include "rphp/analysis/pSourceModule.h"


/**
 * The PHC Passes
 * This is a list of all employed passes and whether we need to port them or not.
 * Invalid_check: we want this as semantic analysis check, i just dunno if we can make use of everything which is there.
 * AST to HIR:
 * Strip_comments: We don't have a notion of comments yet in rphp.
 * Strip_unparser_attributes: We don't have an unparser back to php...
 * Note_top_level_declarations: I'm unsure how we deal with declarations at all. and we don't have classes/interfaces in the
 * 								AST currently anyways...
 * 								Note: We got a codegen pass for decls as well, from the old rphp design.
 * Constant_folding: a) we don't have a match() function in rphp b) We don't call to zend php for folding constant expressions
 * Remove_concat_null: Our parser shouldn't create these concats, so this issue is phc specific.
 * Desugar: - and/or renaming to && and || happens already in our parser
 * 			- we don't have classes yet (no need to replace self:: and parent:: yet)
 * 			- same for interfaces
 *			- we don't store cast types as strings but as enum, so we don't have to normalize them.
 *			- we already generate the same AST for return; and return NULL;
 *			- i'm unsure whether we want to transform - x to 0 - x
 * Split_multiple_arguments: We got a better facility to iterate over children nodes, so we don't need this imho.
 * Split_unset_isset: this one makes more sense, but we need a facility to return an expression list first (sth block-like)
 * Echo_split: we definitly want this for the reason given in Echo_split.cpp, but see S-u-i...
 * Early_lower_control_flow: we want this at least partially because it simplifies loop codegen later on, but we've to be
 * 							 super careful with continue. Probably the transformations by phc can be used, but we want to
 * 							 sure they are optimized later if the loop body doesn't contain a continue;.
 * Lower_expr_flow: We want everything except the comma-seperated list transform. We need an EXPR_LIST anyways, i'd like
 * 					to have one which is not binary but uses a std::vector or sth like that
 * List_shredder: We want this one :)
 * AST_shredder: we definitly want the array assign stuff, but i'm unsure if we need/want the temp var creation for
 * 				 nearly everything.
 * Pre_post_op_shredder: makes sense, but is a bit more complex.
 * Switch_bin_op: should simplify codegen a bit :)
 * Remove_solo_exprs: because we don't have an Eval_expr facility, we can't really do this pass so easily as phc can.
 * 					  I'm unsure whether we want to support this or not.
 */

namespace rphp {

pPassManager::~pPassManager(void) {

    // free passes
    for (queueType::iterator i = passQueue_.begin();
         i != passQueue_.end();
         ++i) {
        delete *i;
    }

}

void pPassManager::run(void) {

    for (queueType::iterator i = passQueue_.begin();
         i != passQueue_.end();
         ++i) {
        (*i)->pre_run();
        (*i)->run();
        (*i)->post_run();
    }

}


} // namespace
