/*===================================================================================*/
/*************************************************************************************/
/**
 * @file        utest_add_to_list_01.h
 * @brief       Test Case #1 for add_to_list() API: Add single node
 * @details     Test Case #1 checks if add_to_list() handles the single node case
 * @see         https://api.cmocka.org/index.html
 * @author      Ashish Bajaj, ashish@elear.solutions
 * @copyright   Copyright (c) 2017 Elear Solutions Tech Private Limited. All rights
 *              reserved.
 * @license     To any person (the "Recipient") obtaining a copy of this software and
 *              associated documentation files (the "Software"):\n
 *              All information contained in or disclosed by this software is
 *              confidential and proprietary information of Elear Solutions Tech
 *              Private Limited and all rights therein are expressly reserved.
 *              By accepting this material the recipient agrees that this material and
 *              the information contained therein is held in confidence and in trust
 *              and will NOT be used, copied, modified, merged, published, distributed,
 *              sublicensed, reproduced in whole or in part, nor its contents revealed
 *              in any manner to others without the express written permission of
 *              Elear Solutions Tech Private Limited.
 */
/*************************************************************************************/
/*===================================================================================*/
#ifndef UTEST_ADD_TO_LIST_01_INCLUDED
#define UTEST_ADD_TO_LIST_01_INCLUDED

/*************************************************************************************
 *                          INCLUDES                                                 *
 *************************************************************************************/

/*************************************************************************************
 *                          GLOBAL MACROS                                            *
 *************************************************************************************/

/*************************************************************************************
 *                          GLOBAL TYPEDEFS                                          *
 *************************************************************************************/

/*************************************************************************************
 *                          GLOBAL VARIABLES                                         *
 *************************************************************************************/

/*************************************************************************************
 *                          PUBLIC FUNCTIONS                                         *
 *************************************************************************************/

/*************************************************************************************/
/**
 * @brief         Test Case #1 for add_to_list() API: Add Single Node
 * @details       Test Case #1 checks if add single node works
 *<PRE>
 * testscenario   Test Case #1 for create_list() API: Add Single Node
 * testcase       Test Case #1 checks if add_to_list() works correctly for single node add
 * testdata       None
 * expected       returns two unique pointers
 *</PRE>
 */
/************************************************************************************/
void utest_add_to_list_01(void **state);

#endif // UTEST_ADD_TO_LIST_01_INCLUDED





