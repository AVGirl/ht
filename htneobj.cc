/* 
 *	HT Editor
 *	htneobj.cc
 *
 *	Copyright (C) 1999, 2000, 2001 Stefan Weyergraf (stefan@weyergraf.de)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "htatom.h"
#include "htendian.h"
#include "htnewexe.h"
#include "htne.h"
#include "htneobj.h"
#include "httag.h"
#include "htstring.h"
#include "formats.h"

#include "nestruct.h"

#include <stdlib.h>
#include <string.h>

ht_mask_ptable neobj[]=
{
	{"offset",	STATICTAG_EDIT_WORD_LE("00000000")},
	{"size",		STATICTAG_EDIT_WORD_LE("00000002")},
	{"flags",		STATICTAG_EDIT_WORD_LE("00000004")" "STATICTAG_FLAGS("00000004", ATOM_NE_SEGFLAGS_STR)},
	{"minalloc",	STATICTAG_EDIT_WORD_LE("00000006")},
	{0, 0}
};

ht_tag_flags_s ne_segflags[] =
{
	{0,  "[00] data"},
	{1,  "[01] reserved"},
	{2,  "[02] reserved"},
	{3,  "[03] iterated"},
	{4,  "[04] movable"},
	{5,  "[05] sharable"},
	{6,  "[06] preload"},
	{7,  "[07] read only"},
	{8,  "[08] relocations"},
	{9,  "[09] conforming"},
	{10, "[10] pl0"},
	{11, "[11] pl1"},
	{12, "[12] discardable"},
	{13, "[13] 32bit"},
	{14, "[14] huge"},
	{15, "[15] reserved"},
	{0, 0}
};

ht_view *htnesegments_init(bounds *b, ht_streamfile *file, ht_format_group *group)
{
	ht_ne_shared_data *ne_shared=(ht_ne_shared_data *)group->get_shared_data();

	dword h=ne_shared->hdr_ofs;
	ht_uformat_viewer *v=new ht_uformat_viewer();
	v->init(b, DESC_NE_SEGMENTS, VC_EDIT | VC_SEARCH, file, group);
	ht_mask_sub *m=new ht_mask_sub();
	m->init(file, 0);

	register_atom(ATOM_NE_SEGFLAGS, ne_segflags);

	char t[64];

	sprintf(t, "* NE segment table at offset %08x", h+ne_shared->hdr.segtab);
	m->add_mask(t);
	m->add_mask("note: 0 means 65536 for segment size and minalloc");

	file->seek(h+ne_shared->hdr.segtab);
	for (dword i=0; i<ne_shared->hdr.cseg; i++) {
		NE_SEGMENT s;
		file->read(&s, sizeof s);
		create_host_struct(&s, NE_SEGMENT_struct, little_endian);
		sprintf(t, "--- segment %d (%s) ---", i+1, (s.flags & NE_DATA) ? "data" : "code");
		m->add_mask(t);
		m->add_staticmask_ptable(neobj, h+ne_shared->hdr.segtab+i*8, false);
	}
	v->insertsub(m);

	return v;
}

format_viewer_if htnesegments_if = {
	htnesegments_init,
	0
};
