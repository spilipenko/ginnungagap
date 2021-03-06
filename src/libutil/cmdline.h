// Copyright (C) 2010, 2011, Steffen Knollmann
// Released under the terms of the GNU General Public License version 3.
// This file is part of `ginnungagap'.

#ifndef CMDLINE_H
#define CMDLINE_H


/*--- Doxygen file description ------------------------------------------*/

/**
 * @file libutil/cmdline.h
 * @ingroup libutilMisc
 * @brief  This file provides an interface to handle command line
 *         parameters.
 */


/*--- Includes ----------------------------------------------------------*/
#include "util_config.h"
#include <stdbool.h>
#include <stdio.h>


/*--- Typedefs ----------------------------------------------------------*/

/** @brief  The different type of the command line arguments. */
typedef enum {
	/** @brief  No type. */
	CMDLINE_TYPE_NONE,
	/** @brief  Integer type. */
	CMDLINE_TYPE_INT,
	/** @brief  Long integer type. */
	CMDLINE_TYPE_LONG,
	/** @brief  Double type. */
	CMDLINE_TYPE_DOUBLE,
	/** @brief  String type. */
	CMDLINE_TYPE_STRING
} cmdline_type_t;


/*--- ADT handle --------------------------------------------------------*/

/** @brief  The handle of the command line structure. */
typedef struct cmdline_struct *cmdline_t;


/*--- Prototypes of exported functions ----------------------------------*/

/**
 * @brief  This will generate a new cmdline ADT.
 *
 * @param  numArgs    The number of command line arguments. Note that
 *                    not all the arguments need to be used, this only
 *                    gives the maximum number of arguments the ADT can
 *                    hold.  The actual number of arguments the code is
 *                    using is calculated automatically by the number of
 *                    argument descriptions added to the ADT.
 * @param  numOpts    The number of command line options.  The same note
 *                    as above applies.
 * @param  *progName  The name of the program.
 *
 * @return  A properly set up cmdline ADT.
 */
extern cmdline_t
cmdline_new(int numArgs, int numOpts, const char *progName);


/**
 * @brief  This will delete a given cmdline ADT and free all associated
 *         memory.
 *
 * @param  *cmdline  A pointer to the ADT that should be freed.
 *
 * @return  Returns nothing, however the external variable holding the
 *          ADT will be set to NULL.
 */
extern void
cmdline_del(cmdline_t *cmdline);


/**
 * @brief  Adds a command line option to the ADT.
 *
 * @param  cmdline            The cmdline ADT.
 * @param  *optName           The name of the option, note that no
 *                            white-spaces are allowed in this string.
 * @param  *optDescription    A description of what this option does.
 * @param  *optValueRequired  A flag that states whether the options
 *                            requires the value to be set of whether is
 *                            it optional.
 * @param  optType            The type of the value the option takes.
 *
 * @return  Returns the number of this option or -1 in the case of
 *          errors.
 */
extern int
cmdline_addOpt(cmdline_t      cmdline,
               const char     *optName,
               const char     *optDescription,
               bool           optValueRequired,
               cmdline_type_t optType);


/**
 * @brief  Adds a command line argument to the ADT.
 *
 * @param  cmdline          The cmdline ADT.
 * @param  *argDescription  A description of what this argument does.
 * @param  argType          The type of the value the argument takes.
 *
 * @return  Returns the number of this argument or -1 in the case of
 *          errors.
 */
extern int
cmdline_addArg(cmdline_t      cmdline,
               const char     *argDescription,
               cmdline_type_t argType);


/**
 * @brief  This will print a usage instruction to the given FILE stream.
 *
 * @param  cmdline  The cmdline ADT.
 * @param  *fout    The stream to write to.
 *
 * @return  Returns nothing.
 */
extern void
cmdline_printHelp(cmdline_t cmdline,
                  FILE      *fout);


/**
 * @brief  This will parse the command line.
 *
 * @param  cmdline  The cmdline ADT.
 * @param  argc     The number of command line parameters.
 * @param  **argv   The tokenized command line parameters.
 *
 * @return  Returns nothing, however, the cmdline ADT will be updated.
 */
extern void
cmdline_parse(cmdline_t cmdline,
              int       argc,
              char      **argv);


/**
 * @brief  This will verify whether a cmdline ADT is correct, ie.
 *         whether all requried parameters are set.
 *
 * @param  cmdline  The commandline ADT.
 *
 * @return  Returns true if the ADT is set correctly, otherwise false is
 *          returned.
 */
extern bool
cmdline_verify(cmdline_t cmdline);


/**
 * @brief  This will check whether a command line option is set.
 *
 * @param  cmdline  The cmdline ADT.
 * @param  optNum   The number of the option that should be checked.
 *
 * @return  Returns true if the option is set, false otherwise.
 */
extern bool
cmdline_checkOptSetByNum(cmdline_t cmdline, int optNum);


/**
 * @brief  This will return the value of command line option.
 *
 * @param  cmdline  The cmdline ADT.
 * @param  optNum   The option number for which to retrieve the value.
 * @param  *val     A pointer to an external variable that will recieve
 *                  the value.  Its type must coincide with the type
 *                  specified for the value.  Note that if a string is
 *                  returned, the calling function is responsible for
 *                  freeing the associated memory.
 *
 * @return  This will return true if the value could be set correctly,
 *          otherwise false is returned, indicating that the passed
 *          variable has not been updated.
 */
extern bool
cmdline_getOptValueByNum(cmdline_t cmdline, int optNum, void *val);


/**
 * @brief  This will check whether a command line argument is set.
 *
 * @param  cmdline  The cmdline ADT.
 * @param  argNum   The number of the argument that should be checked.
 *
 * @return  Returns true if the option is set, false otherwise.
 */
extern bool
cmdline_checkArgSetByNum(cmdline_t cmdline, int argNum);


/**
 * @brief  This will return the value of command line argument.
 *
 * @param  cmdline  The cmdline ADT.
 * @param  argNum   The argument number for which to retrieve the value.
 * @param  *val     A pointer to an external variable that will recieve
 *                  the value.  Its type must coincide with the type
 *                  specified for the value.  Note that if a string is
 *                  returned, the calling function is responsible for
 *                  freeing the associated memory.
 *
 * @return  This will return true if the value could be set correctly,
 *          otherwise false is returned, indicating that the passed
 *          variable has not been updated.
 */
extern bool
cmdline_getArgValueByNum(cmdline_t cmdline, int argNum, void *val);


#endif
