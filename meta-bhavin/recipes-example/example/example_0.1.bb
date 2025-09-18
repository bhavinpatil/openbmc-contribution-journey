SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*  Example recipe created by bitbake-layers   *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

addtask display_banner before do_build


# Variable Assignments
## Types of Variable Assignments

# COMMANDS
# bitbake -e <RECIPE_NAME> | grep ^<VARIABLE_NAME>

# <RECIPE_NAME> = example
# <VARIABLE_NAME> = TEST, A, B, SUMMARY, DESCRIPTION, LICENSE, STAGING_ETCDIR_NATIVE

# '?='  : This is used to assign the default value to variable. It can be overridden.

TEST1 ?= "This is a test variable 1"
TEST1 ?= "This is a test variable 2"
TEST1 ?= "This is a test variable 3"
TEST1 ?= "This is a test variable 4"

# ---------------------------------------------------------------------------------

# '??='  : used to assign the default value to vairbale, but it is weak assignment, it can be overridden , if multiple assignments are done, the last one will be considerd.

TEST2 ??= "This is a test variable 1"
TEST2 ??= "This is a test variable 2"
TEST2 ??= "This is a test variable 3"
TEST2 ??= "This is a test variable 4" 

# ---------------------------------------------------------------------------------

TEST3 ??= "FOO"
TEST3 ?= "BAR"
TEST3 ?= "VAL"
TEST3 ??= "VAR"

# ?= has higher priority that ??= , so first and last i.e. FOO and VAR will be ignored 
# and ?= will always considers first assiged value so BAR will be the final value for TEST3


#  black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^TEST1=
#  TEST1="This is a test variable 1"
#  black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^TEST2=
#  TEST2="This is a test variable 4"
#  black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^TEST3=
#  TEST3="BAR"
# =================================================================================

# '='   : This is a simple variable assignment. It requires spaec " " and spaces are significant, but variable are expanded at the end.

# Overrides
TEST4 ?= "This is A variable"
TEST4 = "This is A variable with = assignment"

TEST4 = "FOO"
TEMP1 = "${A}"
TEST4 = "BAR"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^A=
# A="This is A variable with = assignment"
# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^B=
# B="BAR"

# we got B as BAR because variable are expanded at the end so at the end A is BAR

# ---------------------------------------------------------------------------------

# ':='  : it is an immediate variable expansion, value assigned is expanded immediately.

# Override
# TEST5 ?= "FOO"
# TEST5 := "BAR"

TEST5 = "FOO"
TEMP2 := "${TEST5}"
TEST5 = "BAR"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^TEMP2=
# TEMP2="FOO"

# it will not wait to end to assign or expand value to TEMP2 , TEMP2 will be assigned the value immediately

# =================================================================================

# +=  : it is used to append the value to variable , inserts the space between current and appended value

APPEND1 = "Hello"
APPEND1 += "World"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^TEST6
# TEST6="Hello World"
APPEND1 ?= "Again"
APPEND1 += "Hello"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^TEST7
# TEST7="Again Hello"

# ---------------------------------------------------------------------------------

# =+  :used to prepent value to a variable

PREPEND = "Second"
PREPEND =+ "First"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^PREPEND
# PREPEND="First Second"


PREPEND2 ??= "VAL"
PREPEND2 =+ "FOO"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^PREPEND2
# PREPEND2="FOO "


# =================================================================================

# .=  : append value to a variable without inserting space and take effect immediately

APPEND3 = "Hola"
APPEND3 .= "Mundo" 

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^APPEND3
# APPEND3="HolaMundo"

# ---------------------------------------------------------------------------------

# =.  : prepend value to a variable without inserting space and take effect immediately

PREPEND3 = "Senoire"
PREPEND3 =. "Monor"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^PREPEND3
# PREPEND3="MonorSenoire"


# =================================================================================

# :append  : appends the value and inserts no space in between and effects are applied at variable expansion time rather than being immediately 

APPEND4 = "Don"
APPEND4:append = "Hector"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^APPEND4
# APPEND4="DonHector"

APPEND5 = "Los"

#effect does not takes immediately and appends with not space
APPEND5:append = "Harmanos"

#effact takes immediately and appends with space
APPEND5 += "Polos"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^APPEND5
# APPEND5="Los PolosHarmanos"

# ---------------------------------------------------------------------------------

# :prepend    : prepends values to the variable with no space and effects are applied at expansion, means end time

PREPEND4 = "FOO"
PREPEND4:prepend = "TOILET"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^PREPEND4
# PREPEND4="TOILETFOO"

PREPEND5 = "Tress"
PREPEND5:prepend = "Uno"
PREPEND5 =+ "Dos"

# -----Tress =+ Dos = Dos Tress
# -----Dos Tress :prepend = Uno = UnoDos Tress 

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^PREPEND5
# PREPEND5="UnoDos Tress"

# ---------------------------------------------------------------------------------

# :remove     : removes value from list specifically removes all occurances of that value

REMOVE1 = "A B A B A A C"
REMOVE1:remove ="A"

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^REMOVE1
# REMOVE1=" B  B   C"

REMOVE2 = "Hi How Are You"
REMOVE2:remove = "Today"
REMOVE2 += "Today"

# first += will add the Today then it will remove from REMOVE2 

# black@bhavin:~/openbmc/build/qemuarm$ bitbake -e example | grep ^REMOVE2
# REMOVE2="Hi How Are You "