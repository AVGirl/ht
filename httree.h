/* 
 *	HT Editor
 *	httree.h
 *
 *	Copyright (C) 1999-2002 Sebastian Biallas (sb@web-productions.de)
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

#ifndef HT_TREE
#define HT_TREE

#include "htdata.h"
#include "htobj.h"
#include "global.h"

struct ht_treeview_data {
	DDECL_PTR(void *, selected);
};

class ht_treeview: public ht_view {
public:
		int delta_x, delta_y;
		int maxsize_x, maxsize_y;
		int			foc;
		void			*selected; // only for getdata()

			   void	init(bounds *b, char *desc);
		virtual void	done();
		virtual void	adjust(void *node, bool expand) = 0;
			   int 	create_graph(int *s, void *node, int level, int lines, int width, int endwidth, int *Chars);
			   void   collapse_all(void *node);
		virtual int	count_children(void *node);
		virtual void	draw();
			   void	draw_r(void *node, int level, int *pos, dword lines);
			   void   expand_all(void *node);
		virtual void   *get_child(void *node, int i) = 0;
		virtual void	getdata(ht_object_stream *s);
		virtual int 	get_graph(int *s, void *node, int level, int lines);
		virtual void	*get_next_node(void *node) = 0;
			   void   *get_node(int i);
			   void   *get_node_r(void *node, int *i);
		virtual void	*get_prev_node(void *node) = 0;
		virtual void	*get_root() = 0;
		virtual char	*get_text(void *node) = 0;
		virtual void	handlemsg(htmsg *msg);
		virtual bool	has_children(void *node) = 0;
		virtual void	has_focused(int i);
		virtual bool	is_expanded(void *node) = 0;
		virtual bool	is_selected(int i);
			   void	scroll_to(int x, int y);
		virtual void	select_node(void *node); // stub
		virtual void	setdata(ht_object_stream *s);
			   void	set_limit(int x, int y);
			   void	update();
			   void	update_r(void *node, int level, int *pos, int *x);
private:
			   void   adjust_focus(int Focus);
};

struct static_node {
	static_node	*next, *prev, *child;
	char			*text;
	bool			expanded;
	ht_data		*data;
};

class ht_static_treeview: public ht_treeview {
public:
		static_node	*root;
			   void	init(bounds *b, char *desc);
		virtual void	done();
			   void	*add_child(void *node, char *text, ht_data *Data=NULL);
			   void	*add_node(static_node **node, char *text, ht_data *Data=NULL);
		virtual void	adjust(void *node, bool expand);
		static_node 	*create_node(char *text, static_node *prev, ht_data *Data=NULL);
		virtual void   *get_child(void *node, int i);
		virtual void	*get_next_node(void *node);
		virtual void	*get_prev_node(void *node);
		virtual void	*get_root();
		virtual char	*get_text(void *node);
		virtual bool	has_children(void *node);
		virtual bool	is_expanded(void *node);
		virtual void	select_node(void *node);
};

#endif