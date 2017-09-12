/*
    test_cases.c -- Execution of specific meshlink black box test cases
    Copyright (C) 2017  Guus Sliepen <guus@meshlink.io>
                        Manav Kumar Mehta <manavkumarm@yahoo.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <signal.h>
#include "logger.h"
#include "util.h"
#include "list.h"

/*************************************************************************************
 *                          LOCAL MACROS                                             *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL TYPEDEFS                                           *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL PROTOTYPES                                         *
 *************************************************************************************/
static void utest_sig_handler(int sig);

/*************************************************************************************
 *                          GLOBAL VARIABLES                                         *
 *************************************************************************************/

/*************************************************************************************
 *                          LOCAL VARIABLES                                          *
 *************************************************************************************/
static sigjmp_buf stack;
static int checkSum = 0;

/*************************************************************************************
 *                          PRIVATE FUNCTIONS                                        *
 *************************************************************************************/
/**************************************************************************************************
Name        : utest_sig_handler()
Input(s)    : signal that fired to cause this handler to execute
Output(s)   : void
Description : Uses longjmp to restore the stack that was saved by previous call of setjmp().
              Note that after longjmp() is completed, the program excutes as if the corresponding
              call of setjmp has just returned with the value specified in longjmp()
***************************************************************************************************/
static void utest_sig_handler(int sig) {
  longjmp(stack, 1);

  return;
}

/**************************************************************************************************
Name        : data_free()
Input(s)    : data: to free from the node currently being iterated
Output(s)   : None
Description : This functions is called for every node in the linked list. It should free the data
***************************************************************************************************/
static void data_free(void *data) {
  checkSum += *((int *)data);
  // typically we would free the data here
  // but since we are using an array for the unit test framework
  // a free would cause a crash

  return;
}

/*************************************************************************************
 *                          PUBLIC FUNCTIONS                                         *
 *************************************************************************************/
/*************************************************************************************
 *          Refer to the header file for a detailed description                      *
 *************************************************************************************/
void utest_add_to_list_01(void **state) {
  struct sigaction sa;
  list_t **listHandles = *state;
  int data[6] = { 00, 11, 22, 33, 44, 55 };
  int nodeCount;
  int *testVal;

  /* Setup Signal Handler to handle SIGSEGV, and it should block every signal during handling */
  sa.sa_handler = utest_sig_handler;
  sigfillset(&sa.sa_mask);
  if (sigaction(SIGSEGV, &sa, NULL) == -1) {
    debug_log(LOG_ERR, "Error: unable to handle SIGSEGV", NULL);
    cleanup_and_exit();
  }

  /* Use setjmp to save the stack context in 'stack' pointer variable so to handle
   * the case where the FUT crashes and the signal handler returns 1
   * NOTE: setjmp(stack) will always return 0 immediately, and 1 is returned only from
   *       the signal handler using longjmp(stack, 1)
   */
  if (setjmp(stack) == 0) {
    listHandles[0] = create_list();

    /*
     * Make sure list is empty, then add single node and make sure
     * headNode->data == tailNode->data
     * count == 1
     */
    assert_int_equal(is_list_empty(listHandles[0]), 1);

    nodeCount = add_to_list(listHandles[0], &data[3]);
    assert_int_equal(nodeCount, 1);

    testVal = (int *)get_list_head_node(listHandles[0]);
    assert_int_equal(*testVal, 33);

    testVal = (int *)get_list_tail_node(listHandles[0]);
    assert_int_equal(*testVal, 33);

    assert_int_equal(is_list_empty(listHandles[0]), 0);

    /* destroy_list */
    checkSum = 0;
    debug_log(LOG_DEBUG, "destroying the list", NULL);
    destroy_list(listHandles[0], data_free);
    assert_int_equal(checkSum, 33);
  } else {
    fail_msg("Seg Fault in create_list()");
  }

  return;
}






