/*
 * main.cpp
 *
 *  Created on: May 19, 2014
 *      Author: dmarce1
 */

#include "xtree.hpp"

XTREE_INSTANTIATE(8,8,8);

void test_locality_server(int depth);

HPX_PLAIN_ACTION(test_locality_server, action_test_locality_server);

int hpx_main() {
	xtree::server::initialize().get();
	xtree::server::increment_load();
	xtree::node_type root_node;
	root_node.refine();
	return hpx::finalize();
}

void test_locality_server(int depth) {
	hpx::id_type next_locality;
	printf("Starting thread on %i with load %i\n", hpx::get_locality_id(), xtree::server::get_load());
	//sleep( rand() % 2);
	if (depth > 0) {
		for (int i = 0; i < 2; i++) {
			next_locality = xtree::server::increment_load().get();
			hpx::apply<action_test_locality_server>(next_locality, depth - 1);
		}

	}
	//sleep( rand() % 10);
	//xtree::server::decrement_load();
//	printf("Stopping thread on %i with load %i\n", hpx::get_locality_id(), xtree::server::get_load());

}
