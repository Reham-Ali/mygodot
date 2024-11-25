/**************************************************************************/
/*  override.cpp                                                          */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "thirdparty/mimalloc/include/mimalloc.h"

extern "C" {

// Dummy symbol to ensure linkage.
void mimalloc_godot_force_override() {}

// ---

// Since the mechanism for overriding the C memory functions on Windows
// that mimalloc provides only works when building as a DLL, we have to
// provide these manual overrides and hope they cover everything.

char *strdup(const char *str) {
	return mi_strdup(str);
}

void *calloc(size_t count, size_t size) {
	return mi_calloc(count, size);
}

void cfree(void *p) {
	mi_free(p);
}

void *_expand(void *p, size_t newsize) {
	return mi_expand(p, newsize);
}

void free(void *ptr) {
	mi_free(ptr);
}

void *malloc(size_t size) {
	return mi_malloc(size);
}

size_t malloc_size(const void *p) {
	return mi_usable_size(p);
}

size_t malloc_good_size(size_t size) {
	return mi_good_size(size);
}

size_t malloc_usable_size(const void *p) {
	return mi_usable_size(p);
}

int posix_memalign(void **p, size_t alignment, size_t size) {
	return mi_posix_memalign(p, alignment, size);
}

__declspec(restrict) void *memalign(size_t alignment, size_t size) {
	return mi_memalign(alignment, size);
}

__declspec(restrict) void *valloc(size_t size) {
	return mi_valloc(size);
}

__declspec(restrict) void *pvalloc(size_t size) {
	return mi_pvalloc(size);
}

__declspec(restrict) void *aligned_alloc(size_t alignment, size_t size) {
	return mi_aligned_alloc(alignment, size);
}

void *reallocarray(void *p, size_t count, size_t size) {
	return mi_reallocarray(p, count, size);
}

int reallocarr(void *p, size_t count, size_t size) {
	return mi_reallocarr(p, count, size);
}

void *aligned_recalloc(void *p, size_t newcount, size_t size, size_t alignment) {
	return mi_aligned_recalloc(p, newcount, size, alignment);
}

void *aligned_offset_recalloc(void *p, size_t newcount, size_t size, size_t alignment, size_t offset) {
	return mi_aligned_offset_recalloc(p, newcount, size, alignment, offset);
}

#if 0 // This one doesn't work.
__declspec(restrict) unsigned short *wcsdup(const unsigned short *s) {
	return mi_wcsdup(s);
}
#endif

__declspec(restrict) unsigned char *mbsdup(const unsigned char *s) {
	return mi_mbsdup(s);
}

int dupenv_s(char **buf, size_t *size, const char *name) {
	return mi_dupenv_s(buf, size, name);
}

int wdupenv_s(unsigned short **buf, size_t *size, const unsigned short *name) {
	return mi_wdupenv_s(buf, size, name);
}

void free_size(void *p, size_t size) {
	mi_free_size(p, size);
}

void free_size_aligned(void *p, size_t size, size_t alignment) {
	mi_free_size_aligned(p, size, alignment);
}

void free_aligned(void *p, size_t alignment) {
	mi_free_aligned(p, alignment);
}

__declspec(restrict) void *new_aligned(size_t size, size_t alignment) {
	return mi_new_aligned(size, alignment);
}

__declspec(restrict) void *new_nothrow(size_t size) {
	return mi_new_nothrow(size);
}

__declspec(restrict) void *new_aligned_nothrow(size_t size, size_t alignment) {
	return mi_new_aligned_nothrow(size, alignment);
}

__declspec(restrict) void *new_n(size_t count, size_t size) {
	return mi_new_n(count, size);
}

void *new_realloc(void *p, size_t newsize) {
	return mi_new_realloc(p, newsize);
}

void *new_reallocn(void *p, size_t newcount, size_t size) {
	return mi_new_reallocn(p, newcount, size);
}

void *realloc(void *ptr, size_t size) {
	return mi_realloc(ptr, size);
}

} // extern "C"

// ---

// At least, mimalloc provides replacements for C++'s new, etc.,
// in this header that has to be included in a single translation unit.
#include "thirdparty/mimalloc/include/mimalloc-new-delete.h"
