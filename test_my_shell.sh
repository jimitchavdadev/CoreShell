#!/bin/bash

# --- Color Codes for Output ---
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- Helper Function to Print Headers ---
print_header() {
    echo -e "\n${BLUE}======================================================================${NC}"
    echo -e "${BLUE}  $1 ${NC}"
    echo -e "${BLUE}======================================================================${NC}"
}

# --- Pre-flight Check ---
SHELL_EXECUTABLE="./my_shell"
if [ ! -f "$SHELL_EXECUTABLE" ]; then
    echo -e "${RED}Error: Shell executable '$SHELL_EXECUTABLE' not found!${NC}"
    echo -e "${YELLOW}Please compile the project first using 'make'.${NC}"
    exit 1
fi

# ==============================================================================
#  PART 1: AUTOMATED TESTS
# ==============================================================================
# We use a 'here document' to feed commands into the shell's standard input.

print_header "Starting Automated Tests for my_shell"

"$SHELL_EXECUTABLE" <<EOF

# --- 1. Testing External Commands ---
echo ""
echo "--- 1. Testing External Commands ---"
pwd
ls -l

# --- 2. Testing Built-in Commands ---
echo ""
echo "--- 2. Testing Built-in Commands ---"
sysinfo
my_echo This is a test of the 'my_echo' built-in command.
echo "Changing directory to 'src'..."
my_cd src
pwd
echo "Changing back to parent directory..."
my_cd ..
pwd

# --- 3. Testing I/O Redirection (Output) ---
echo ""
echo "--- 3. Testing I/O Redirection ('>') ---"
my_echo "Hello from my_shell into a file!" > test_output.txt
echo "File 'test_output.txt' created. We will verify its contents after the shell exits."

# --- 4. Testing Piping ---
echo ""
echo "--- 4. Testing Piping ('|') ---"
echo "Running 'ls -l | grep .c'"
ls -l | grep .c

# --- 5. Testing Command History ---
echo ""
echo "--- 5. Testing History ---"
history

# --- 6. Testing Job Control (Automated Part) ---
echo ""
echo "--- 6. Testing Job Control ('&', 'jobs', 'kill') ---"
echo "Starting 'countdown 5' in the background..."
countdown 5 &
echo "Starting 'sleep 10' in the background..."
sleep 10 &
echo "Listing current jobs:"
jobs
echo "Killing job %2 (sleep)..."
kill %2
echo "Listing jobs again to see the result:"
jobs
echo "Waiting for the 'countdown' job to finish naturally..."
# Give the countdown job a moment to finish before exiting
sleep 6

# --- Exiting the shell ---
echo ""
echo "Automated tests complete. Exiting my_shell."
my_exit
EOF

# ==============================================================================
#  PART 2: POST-EXECUTION VERIFICATION
# ==============================================================================

print_header "Post-Execution Verification"

# --- Verify Redirection ---
echo -e "${YELLOW}Checking results of I/O Redirection...${NC}"
TEST_FILE="test_output.txt"
EXPECTED_CONTENT="Hello from my_shell into a file!"

if [ -f "$TEST_FILE" ]; then
    FILE_CONTENT=$(cat "$TEST_FILE")
    if [ "$FILE_CONTENT" == "$EXPECTED_CONTENT" ]; then
        echo -e "${GREEN}SUCCESS: Redirection test passed. File '$TEST_FILE' contains the correct content.${NC}"
    else
        echo -e "${RED}FAILURE: Redirection test failed. File content is incorrect.${NC}"
        echo "Expected: '$EXPECTED_CONTENT'"
        echo "Got: '$FILE_CONTENT'"
    fi
    # Cleanup
    rm "$TEST_FILE"
    echo "Cleaned up '$TEST_FILE'."
else
    echo -e "${RED}FAILURE: Redirection test failed. File '$TEST_FILE' was not created.${NC}"
fi


# ==============================================================================
#  PART 3: MANUAL INTERACTIVE TESTS
# ==============================================================================

print_header "Manual Testing Required for Interactive Features"

echo -e "${YELLOW}The script has finished automated tests. Now, please test interactive job control manually.${NC}"
echo "Follow these steps:"
echo ""
echo "  1. Run the shell: ${GREEN}./my_shell${NC}"
echo ""
echo "  2. Start a process in the foreground:"
echo -e "     ${YELLOW}my_shell> sleep 60${NC}"
echo ""
echo "  3. While it's running, press ${GREEN}Ctrl+Z${NC}. The shell should stop the process."
echo ""
echo "  4. List the jobs to see the stopped process:"
echo -e "     ${YELLOW}my_shell> jobs${NC}"
echo ""
echo "  5. Resume the stopped job in the background:"
echo -e "     ${YELLOW}my_shell> bg %1${NC}"
echo ""
echo "  6. Check the jobs list again. It should now be 'Running'."
echo -e "     ${YELLOW}my_shell> jobs${NC}"
echo ""
echo "  7. Bring the job back to the foreground:"
echo -e "     ${YELLOW}my_shell> fg %1${NC}"
echo ""
echo "  8. Now, you can terminate it with ${GREEN}Ctrl+C${NC} or let it finish."
echo ""
echo -e "${GREEN}If all these steps worked, your job control implementation is fully functional!${NC}"