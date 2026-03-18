#!/bin/bash

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"
PASS=0
FAIL=0
TOTAL=0

check() {
	TOTAL=$((TOTAL + 1))
	DESC="$1"
	GOT="$2"
	EXPECTED="$3"
	if [ "$GOT" = "$EXPECTED" ]; then
		PASS=$((PASS + 1))
		echo -e "${GREEN}[OK]${RESET} $DESC"
	else
		FAIL=$((FAIL + 1))
		echo -e "${RED}[KO]${RESET} $DESC"
		echo "  expected: |$EXPECTED|"
		echo "  got:      |$GOT|"
	fi
}

echo "And above all," > /tmp/ft_ssl_testfile
echo "https://www.42.fr/" > /tmp/ft_ssl_website
echo "" > /tmp/ft_ssl_empty
echo -n "no newline" > /tmp/ft_ssl_nonl

echo "=========================================="
echo "          ft_ssl test suite"
echo "=========================================="

# ──────────── MD5 BASIC STDIN ────────────
echo ""
echo "--- MD5 basic stdin ---"

GOT=$(echo "42 is nice" | ./ft_ssl md5)
check "md5 basic stdin" "$GOT" "(stdin)= 35f1d6de0302e2086a4e472266efb3a9"

GOT=$(echo -n "" | ./ft_ssl md5)
check "md5 empty stdin" "$GOT" "(stdin)= d41d8cd98f00b204e9800998ecf8427e"

GOT=$(echo -n "hello" | ./ft_ssl md5)
check "md5 stdin no newline" "$GOT" "(stdin)= 5d41402abc4b2a76b9719d911017c592"

# ──────────── MD5 -p ────────────
echo ""
echo "--- MD5 -p ---"

GOT=$(echo "42 is nice" | ./ft_ssl md5 -p)
check "md5 -p" "$GOT" '("42 is nice")= 35f1d6de0302e2086a4e472266efb3a9'

GOT=$(echo -n "hello" | ./ft_ssl md5 -p)
check "md5 -p no newline" "$GOT" '("hello")= 5d41402abc4b2a76b9719d911017c592'

GOT=$(echo -n "" | ./ft_ssl md5 -p)
check "md5 -p empty" "$GOT" '("")= d41d8cd98f00b204e9800998ecf8427e'

# ──────────── MD5 -q ────────────
echo ""
echo "--- MD5 -q ---"

GOT=$(echo "42 is nice" | ./ft_ssl md5 -q)
check "md5 -q" "$GOT" "35f1d6de0302e2086a4e472266efb3a9"

# ──────────── MD5 -r ────────────
echo ""
echo "--- MD5 -r ---"

GOT=$(./ft_ssl md5 -r /tmp/ft_ssl_testfile)
check "md5 -r file" "$GOT" "53d53ea94217b259c11a5a2d104ec58a  /tmp/ft_ssl_testfile"

# ──────────── MD5 -q -r ────────────
echo ""
echo "--- MD5 -q -r ---"

GOT=$(echo "Pity the living." | ./ft_ssl md5 -q -r)
check "md5 -q -r" "$GOT" "e20c3b973f63482a778f3fd1869b7f25"

# ──────────── MD5 -s ────────────
echo ""
echo "--- MD5 -s ---"

GOT=$(./ft_ssl md5 -s "pity those that aren't following baerista on spotify.")
check "md5 -s" "$GOT" "MD5 (\"pity those that aren't following baerista on spotify.\") = a3c990a1964705d9bf0e602f44572f5f"

GOT=$(./ft_ssl md5 -s "")
check "md5 -s empty" "$GOT" 'MD5 ("") = d41d8cd98f00b204e9800998ecf8427e'

GOT=$(./ft_ssl md5 -s "foo")
check "md5 -s foo" "$GOT" 'MD5 ("foo") = acbd18db4cc2f85cedef654fccc4a4d8'

# ──────────── MD5 FILE ────────────
echo ""
echo "--- MD5 file ---"

GOT=$(./ft_ssl md5 /tmp/ft_ssl_testfile)
check "md5 file" "$GOT" "MD5 (/tmp/ft_ssl_testfile) = 53d53ea94217b259c11a5a2d104ec58a"

# ──────────── MD5 -r -s ────────────
echo ""
echo "--- MD5 -r -s ---"

GOT=$(./ft_ssl md5 -r -s "foo")
check "md5 -r -s" "$GOT" 'acbd18db4cc2f85cedef654fccc4a4d8  "foo"'

# ──────────── MD5 -q -s ────────────
echo ""
echo "--- MD5 -q -s ---"

GOT=$(./ft_ssl md5 -q -s "foo")
check "md5 -q -s" "$GOT" "acbd18db4cc2f85cedef654fccc4a4d8"

# ──────────── MD5 -q file ────────────
echo ""
echo "--- MD5 -q file ---"

GOT=$(./ft_ssl md5 -q /tmp/ft_ssl_testfile)
check "md5 -q file" "$GOT" "53d53ea94217b259c11a5a2d104ec58a"

# ──────────── MD5 COMBOS ────────────
echo ""
echo "--- MD5 combos ---"

GOT=$(echo "be sure to handle edge cases carefully" | ./ft_ssl md5 -p /tmp/ft_ssl_testfile)
EXPECTED='("be sure to handle edge cases carefully")= 3553dc7dc5963b583c056d1b9fa3349c
MD5 (/tmp/ft_ssl_testfile) = 53d53ea94217b259c11a5a2d104ec58a'
check "md5 -p file" "$GOT" "$EXPECTED"

GOT=$(echo "some of this will not make sense at first" | ./ft_ssl md5 /tmp/ft_ssl_testfile)
check "md5 stdin ignored with file" "$GOT" "MD5 (/tmp/ft_ssl_testfile) = 53d53ea94217b259c11a5a2d104ec58a"

GOT=$(echo "but eventually you will understand" | ./ft_ssl md5 -p -r /tmp/ft_ssl_testfile)
EXPECTED='("but eventually you will understand")= dcdd84e0f635694d2a943fa8d3905281
53d53ea94217b259c11a5a2d104ec58a  /tmp/ft_ssl_testfile'
check "md5 -p -r file" "$GOT" "$EXPECTED"

GOT=$(echo "GL HF let's go" | ./ft_ssl md5 -p -s "foo" /tmp/ft_ssl_testfile)
EXPECTED="(\"GL HF let's go\")= d1e3cc342b6da09480b27ec57ff243e2
MD5 (\"foo\") = acbd18db4cc2f85cedef654fccc4a4d8
MD5 (/tmp/ft_ssl_testfile) = 53d53ea94217b259c11a5a2d104ec58a"
check "md5 -p -s file" "$GOT" "$EXPECTED"

# ──────────── MD5 EDGE CASES ────────────
echo ""
echo "--- MD5 edge cases ---"

GOT=$(echo "one more thing" | ./ft_ssl md5 -r -p -s "foo" /tmp/ft_ssl_testfile -s "bar" 2>/tmp/ft_ssl_err)
EXPECTED='("one more thing")= a0bd1876c6f011dd50fae52827f445f5
acbd18db4cc2f85cedef654fccc4a4d8  "foo"
53d53ea94217b259c11a5a2d104ec58a  /tmp/ft_ssl_testfile'
check "md5 -r -p -s file -s bar (stdout)" "$GOT" "$EXPECTED"
GOT_ERR=$(cat /tmp/ft_ssl_err)
EXPECTED_ERR='ft_ssl: md5: -s: No such file or directory
ft_ssl: md5: bar: No such file or directory'
check "md5 -r -p -s file -s bar (stderr)" "$GOT_ERR" "$EXPECTED_ERR"

GOT=$(echo "just to be extra clear" | ./ft_ssl md5 -r -q -p -s "foo" /tmp/ft_ssl_testfile)
EXPECTED="just to be extra clear
3ba35f1ea0d170cb3b9a752e3360286c
acbd18db4cc2f85cedef654fccc4a4d8
53d53ea94217b259c11a5a2d104ec58a"
check "md5 -r -q -p -s file" "$GOT" "$EXPECTED"

# ──────────── MD5 ERROR HANDLING ────────────
echo ""
echo "--- MD5 error handling ---"

GOT=$(./ft_ssl md5 /tmp/ft_ssl_doesnotexist 2>&1)
check "md5 nonexistent file" "$GOT" "ft_ssl: md5: /tmp/ft_ssl_doesnotexist: No such file or directory"

GOT=$(echo "test" | ./ft_ssl md5)
check "md5 no flags = stdin" "$GOT" "(stdin)= d8e8fca2dc0f896fd7cb4cb0031ba249"

GOT=$(./ft_ssl md5 -q /tmp/ft_ssl_empty)
EXPECTED=$(md5sum /tmp/ft_ssl_empty | awk '{print $1}')
check "md5 empty file" "$GOT" "$EXPECTED"

# ──────────── MD5 VS MD5SUM ────────────
echo ""
echo "--- MD5 vs md5sum ---"

for str in "" "a" "abc" "42" "hello world" "message digest"; do
	GOT=$(echo -n "$str" | ./ft_ssl md5 -q)
	EXPECTED=$(echo -n "$str" | md5sum | awk '{print $1}')
	check "md5 vs md5sum: \"$str\"" "$GOT" "$EXPECTED"
done

# ──────────── SHA256 BASIC ────────────
echo ""
echo "--- SHA256 basic ---"

GOT=$(echo -n "" | ./ft_ssl sha256 -q)
check "sha256 empty" "$GOT" "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"

GOT=$(echo -n "hello" | ./ft_ssl sha256 -q)
check "sha256 hello" "$GOT" "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824"

GOT=$(echo -n "abc" | ./ft_ssl sha256 -q)
check "sha256 abc" "$GOT" "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"

# ──────────── SHA256 -s ────────────
echo ""
echo "--- SHA256 -s ---"

GOT=$(./ft_ssl sha256 -s "42 is nice")
check "sha256 -s" "$GOT" 'SHA256 ("42 is nice") = b7e44c7a40c5f80139f0a50f3650fb2bd8d00b0d24667c4c2ca32c88e13b758f'

# ──────────── SHA256 FILE ────────────
echo ""
echo "--- SHA256 file ---"

GOT=$(./ft_ssl sha256 -q /tmp/ft_ssl_website)
check "sha256 -q file" "$GOT" "1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf"

# ──────────── SHA256 -p ────────────
echo ""
echo "--- SHA256 -p ---"

GOT=$(echo "42 is nice" | ./ft_ssl sha256 -p)
check "sha256 -p" "$GOT" '("42 is nice")= a5482539287a4069ebd3eb45a13a47b1968316c442a7e69bc6b9c100b101d65d'

# ──────────── SHA256 -r ────────────
echo ""
echo "--- SHA256 -r ---"

GOT=$(./ft_ssl sha256 -r /tmp/ft_ssl_website)
check "sha256 -r file" "$GOT" "1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf  /tmp/ft_ssl_website"

GOT=$(./ft_ssl sha256 -r -s "foo")
check "sha256 -r -s" "$GOT" '2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae  "foo"'

# ──────────── SHA256 VS SHA256SUM ────────────
echo ""
echo "--- SHA256 vs sha256sum ---"

for str in "" "a" "abc" "42" "hello world" "message digest"; do
	GOT=$(echo -n "$str" | ./ft_ssl sha256 -q)
	EXPECTED=$(echo -n "$str" | sha256sum | awk '{print $1}')
	check "sha256 vs sha256sum: \"$str\"" "$GOT" "$EXPECTED"
done

# ──────────── SHA256 COMBOS ────────────
echo ""
echo "--- SHA256 combos ---"

GOT=$(echo "test" | ./ft_ssl sha256 -p -s "foo" /tmp/ft_ssl_website)
EXPECTED='("test")= f2ca1bb6c7e907d06dafe4687e579fce76b37e4e93b7605022da52e6ccc26fd2
SHA256 ("foo") = 2c26b46b68ffc68ff99b453c1d30413413422d706483bfa0f98a5e886266e7ae
SHA256 (/tmp/ft_ssl_website) = 1ceb55d2845d9dd98557b50488db12bbf51aaca5aa9c1199eb795607a2457daf'
check "sha256 -p -s file" "$GOT" "$EXPECTED"

# ──────────── ERROR HANDLING ────────────
echo ""
echo "--- Error handling ---"

GOT=$(./ft_ssl foo 2>&1)
check "invalid command" "$GOT" "ft_ssl: Error: 'foo' is an invalid command."

GOT=$(./ft_ssl "" 2>&1)
check "empty command" "$GOT" "ft_ssl: Error: '' is an invalid command."

# ──────────── MULTIPLE FILES ────────────
echo ""
echo "--- Multiple files ---"

HASH_WEBSITE=$(md5sum /tmp/ft_ssl_website | awk '{print $1}')
GOT=$(./ft_ssl md5 /tmp/ft_ssl_testfile /tmp/ft_ssl_website)
EXPECTED="MD5 (/tmp/ft_ssl_testfile) = 53d53ea94217b259c11a5a2d104ec58a
MD5 (/tmp/ft_ssl_website) = $HASH_WEBSITE"
check "md5 multiple files" "$GOT" "$EXPECTED"

# ──────────── FILE WITHOUT NEWLINE ────────────

GOT=$(./ft_ssl md5 -q /tmp/ft_ssl_nonl)
EXPECTED=$(md5sum /tmp/ft_ssl_nonl | awk '{print $1}')
check "md5 file no trailing newline" "$GOT" "$EXPECTED"

# ──────────── LONG STRINGS ────────────
echo ""
echo "--- Long strings ---"

LONG="1234567890123456789012345678901234567890123456789012345678901234567890"
GOT=$(echo -n "$LONG" | ./ft_ssl md5 -q)
EXPECTED=$(echo -n "$LONG" | md5sum | awk '{print $1}')
check "md5 long string" "$GOT" "$EXPECTED"

GOT=$(echo -n "$LONG" | ./ft_ssl sha256 -q)
EXPECTED=$(echo -n "$LONG" | sha256sum | awk '{print $1}')
check "sha256 long string" "$GOT" "$EXPECTED"

# ──────────── SUMMARY ────────────
echo ""
echo "=========================================="
echo -e "  Results: ${GREEN}${PASS} passed${RESET}, ${RED}${FAIL} failed${RESET}, ${TOTAL} total"
echo "=========================================="

rm -f /tmp/ft_ssl_testfile /tmp/ft_ssl_website /tmp/ft_ssl_empty /tmp/ft_ssl_nonl /tmp/ft_ssl_err
exit $FAIL