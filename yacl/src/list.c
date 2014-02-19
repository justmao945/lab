/*
 * Copyright (c) 2012, Mao Jianjun <justmao945@gmail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN 
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yacl/list.h>

/*
 * links two nodes: lhs <--> rhs.
 * @param lhs, rhs (list_ref_t)
 * @return lhs
 */
#undef  __link_nodes
#define __link_nodes(lhs, rhs) \
  ( (lhs)->next = (rhs), (rhs)->prev = (lhs) )


/* 
 * Inserts `node` before `pos`.
 * @param pos, node (list_ref_t)
 * @return node
 */
#undef  __insert_before
#define __insert_before(pos, node) \
  ( __link_nodes((pos)->prev, node), __link_nodes(node, pos) )


/* 
 * Inserts `node` after `pos`.
 * @param pos, node (list_ref_t)
 * @return
 */
#undef  __insert_after
#define __insert_after(pos, node) \
  ( __link_nodes(node, (pos)->next), __link_nodes(pos, node) )

/*
 * Return node byte size.
 * @param list (list_t*)
 */
#undef  __node_size
#define __node_size(list) ((list)->Object->_sizeof_ + sizeof(struct __list_ref_t))

/**
 * Advance the node by `n` elements.
 * Return advanced node.
 */
static list_ref_t
__advance_node(list_ref_t node, ptrdiff_t n)
{
  if( n > 0 ){
    while(n--) node = node->next;
  } else {
    n = -n;
    while(n--) node = node->prev;
  }
  return node;
}


/*
 * Counts the number of elements between [first, last).
 */
static size_t
__count_nodes(list_ref_t first, list_ref_t last)
{
  register size_t n = 0;
  while( first != last ){
    first = first->next;
    n++;
  }
  return n;
}


/*
 * Reverses the order of nodes between `(before_first, last)` and return `first`.
 */
static list_ref_t
__reverse_nodes(list_ref_t before_first, list_ref_t last)
{
  register list_ref_t first, next;

  /* exchange `prev` and `next` between `(before_first, last)` */
  first = before_first->next;
  while(first != last){
    next = first->next;
    first->next = first->prev;
    first->prev = next;
    first = next;
  }

  next = before_first->next;
  __link_nodes(before_first, last->prev);
  __link_nodes(next, last);
  return before_first;
}


/*
 * Frees nodes between `[first, last)`.
 */
static void
__free_nodes(list_t* list, list_ref_t first, list_ref_t last)
{
  register list_ref_t next, prev;

  prev = first->prev;

  if(list->Object->_del_){
    while(first != last){
      next = first->next;
      list->Object->_del_(first->value);
      list->Alloc->free(list->alloc, first);
      first = next;
    }
  }else{
    while(first != last){
      next = first->next;
      list->Alloc->free(list->alloc, first);
      first = next;
    }
  }
  __link_nodes(prev, last);
}


/* list reference */

static list_ref_t*
_ref_initcp_(list_ref_t *self, const list_ref_t *other)
{
  assert(self);
  assert(other);
  *self = *other;
  return self;
}

static object_t*
_ref_value(list_ref_t ref)
{
  return (object_t*)ref->value;
}

static list_ref_t
_ref_next(list_ref_t ref)
{
  assert(ref);
  return ref->next;
}

static list_ref_t
_ref_next_n(list_ref_t ref, ptrdiff_t n)
{
  return __advance_node(ref, n);
}

static ptrdiff_t
_ref_dist(list_ref_t first, list_ref_t last)
{
  return __count_nodes(first, last);
}

static list_ref_t
_ref_prev(list_ref_t ref)
{
  return ref->prev;
}

static list_ref_t
_refr_next_n(list_ref_t ref, ptrdiff_t n)
{
  return __advance_node(ref, -n);
}

static ptrdiff_t
_refr_dist(list_ref_t first, list_ref_t last)
{
  return __count_nodes(last, first);
}

const list_ref_z ListRef = 
{
  ._sizeof_ = sizeof(list_ref_t),
  ._init_   = NULL,
  ._initcp_ = _ref_initcp_,
  ._del_    = NULL,
  .value    = _ref_value,
  .next     = _ref_next,
  .next_n   = _ref_next_n,
  .dist     = _ref_dist,
  .prev     = _ref_prev,
};

const list_ref_z ListRefR = 
{
  ._sizeof_ = sizeof(list_ref_t),
  ._init_   = NULL,
  ._initcp_ = _ref_initcp_,
  ._del_    = NULL,
  .value    = _ref_value,
  .next     = _ref_prev,
  .next_n   = _refr_next_n,
  .dist     = _refr_dist,
  .prev     = _ref_next,
};


/* -- list -- */

static list_t*
_init_(list_t* self, const object_z* Object)
{
  assert(self);
  assert(Object);
  assert(Object->_sizeof_);
  assert(Object->_initcp_);

  self->Object = Object;
  self->alloc  = &_alloc_;
  self->Alloc  = &Alloc;

  /* FIXME: Use Alloc->malloc ? */
  self->bbeg = (list_ref_t)malloc(__node_size(self));
  if( self->bbeg == NULL ){
    fprintf(stderr, "List._init_(): malloc failed\n");
    return self;
  }
  __link_nodes(self->bbeg, self->end); /* empty list */
  return self;
}

static list_t*
_swap(list_t* self, list_t* other)
{
  list_t t;
  assert(self);
  assert(other);
  t = *self;
  *self = *other;
  *other = t;
  return self;
}

static list_t* 
_del_(list_t* self)
{
  assert(self);
  __free_nodes(self, self->bbeg->next, self->end);
  free(self->bbeg);
  return (list_t*)memset(self, 0, sizeof(list_t));
}

static list_t*
_clear(list_t* self)
{
  assert(self);
  __free_nodes(self, self->bbeg->next, self->end);
  __link_nodes(self->bbeg, self->end);
  return self;
}

static list_t*
_push_back(list_t* self, const object_t* value)
{
  list_ref_t ref;
  assert(self);

  ref = (list_ref_t)self->Alloc->malloc(self->alloc, __node_size(self));
  if( ref == NULL ){
    fprintf(stderr,  "List.push_back(): malloc failed.\n");
    return self;
  }
  self->Object->_initcp_(ref->value, value);
  __insert_before(self->end, ref);
  return self;
}


static list_t*
_initcp_(list_t* self, const list_t* other)
{
  list_ref_t ref;

  assert(self);
  assert(other);

  self->Object = other->Object;
  self->Alloc = other->Alloc;
  self->alloc = other->alloc;

  /* FIXME: Use Alloc->malloc ? */
  self->bbeg = (list_ref_t)malloc(__node_size(self));
  if( self->bbeg == NULL ){
    fprintf(stderr, "List._initcp_(): malloc failed\n");
    return self;
  }
  __link_nodes(self->bbeg, self->end); /* empty list */

  ref = other->bbeg->next;
  while( ref != other->end ){
    _push_back(self, ref->value);
    ref = ref->next;
  }
  return self;
}

static object_t*
_get(const list_t* self, ptrdiff_t pos)
{
  list_ref_t ref;
  assert(self);

  if( pos >= 0 ){
    ref = self->bbeg->next;
    while( ref != self->end && pos-- > 0 ) ref = ref->next;
  }else{
    ref = self->bbeg->prev;
    while( ref != self->end && pos++ < 0 ) ref = ref->prev;
  }
  return ref == self->end ? NULL : (object_t*)ref->value;
}

static list_t*
_set(list_t* self, ptrdiff_t pos, const object_t* value)
{
  object_t *data;

  assert(self);
  assert(value);
  
  if( (data = _get(self, pos)) != NULL ){
    if(self->Object->_del_) self->Object->_del_(data);
    self->Object->_initcp_(data, value);
  }
  return self;
}

static bool
_empty(const list_t* self)
{
  assert(self);
  return self->bbeg->next == self->end;
}

static object_t*
_front(const list_t* self)
{
  assert(self);
  assert( ! _empty(self) );
  return self->bbeg->next->value;
}

static object_t*
_back(const list_t* self)
{
  assert(self);
  assert( ! _empty(self) );
  return self->end->prev->value;
}

static list_ref_t
_begin(const list_t* self)
{
  assert(self);
  return self->bbeg->next;
}

static list_ref_t
_end(const list_t* self)
{
  assert(self);
  return self->end;
}

static list_ref_t
_rbegin(const list_t* self)
{
  assert(self);
  return self->end->prev;
}

static list_ref_t
_rend(const list_t* self)
{
  assert(self);
  return self->bbeg;
}

static list_ref_t
_find(const list_t* self, cmp_f* eq, const object_t* data)
{
  list_ref_t beg;
  assert(self);
  assert(eq);

  beg = self->bbeg->next;
  while(beg != self->end){
    if( eq(beg->value, data) )
      return beg;
    beg = beg->next;
  }
  return self->end;
}

static size_t
_size(const list_t* self)
{
  assert(self);
  return __count_nodes(self->bbeg->next, self->end);
}


static list_ref_t
_insert(list_t* self, list_ref_t pos, const object_t* value)
{
  list_ref_t ref;

  assert(self);
  assert(pos);
  assert(value);

  ref = (list_ref_t)self->Alloc->malloc(self->alloc, __node_size(self));
  if ( ref == NULL ){
    fprintf(stderr, "List.insert():malloc failed.\n");
    return pos;
  }
  self->Object->_initcp_(ref->value, value);
  __insert_before(pos, ref);
  return ref;
}

static list_ref_t
_insert_n(list_t* self, list_ref_t pos, size_t n, const object_t* data)
{
  size_t i;
  assert(self);
  assert(pos);

  i = 0;
  while (i < n){
    _insert(self, pos, data);
    data += self->Object->_sizeof_;
    i++;
  }
  return n == 0 ? pos : pos->prev;
}

static list_ref_t
_insert_r(list_t* self, list_ref_t pos, const ref_z* Ref, ref_t first, ref_t last)
{
  ref_t it;

  assert(self);
  assert(pos);
  assert(Ref);

  it = first;
  while( it != last ){
    _insert(self, pos, Ref->value(it));
    it = Ref->next(it);
  }
  return first == last ? pos : pos->prev;
}

static list_ref_t
_erase(list_t* self, list_ref_t pos)
{
  assert(self);
  assert(pos);

  if( pos != self->end ){
    list_ref_t ret = pos->next;
    __link_nodes(pos->prev, pos->next);
    if(self->Object->_del_) self->Object->_del_(pos->value);
    self->Alloc->free(self->alloc, pos);
    return ret;
  } else {
    return pos;
  }
}

static list_ref_t
_erase_r(list_t* self, list_ref_t first, list_ref_t last)
{
  assert(self);
  assert(first != self->end);
  __link_nodes(first->prev, last);
  __free_nodes(self, first, last);
  return last;
}

static list_t*
_pop_back(list_t* self)
{
  list_ref_t back;

  assert(self);
  assert( ! _empty(self) );

  back = self->end->prev;
  __link_nodes(back->prev, back->next);
  if(self->Object->_del_) self->Object->_del_(back->value);
  self->Alloc->free(self->alloc, back);
  return self;
}

static list_t*
_push_front(list_t* self, const object_t* value)
{
  list_ref_t ref;

  assert(self);
  assert(value);
  
  ref = (list_ref_t)self->Alloc->malloc(self->alloc, __node_size(self));
  if( ref == NULL ){
    fprintf(stderr,  "List.push_front(): malloc failed.\n");
    return self;
  }
  self->Object->_initcp_(ref->value, value);
  __insert_after(self->bbeg, ref);
  return self;
}

static list_t*
_pop_front(list_t* self)
{
  list_ref_t front;

  assert(self);
  assert( ! _empty(self) );

  front = self->bbeg->next;
  __link_nodes(front->prev, front->next);
  if(self->Object->_del_) self->Object->_del_(front->value);
  self->Alloc->free(self->alloc, front);
  return self;
}

static list_t*
_resize(list_t* self, size_t n, const object_t* value)
{
  size_t i;
  list_ref_t ref, prev;

  assert(self);

  i = _size(self);
  while( i < n){
    _push_back(self, value);
    i++;
  }

  prev = ref = self->end->prev;
  if(self->Object->_del_){
    while(n < i){
      prev = ref->prev;
      self->Object->_del_(ref->value);
      self->Alloc->free(self->alloc, ref);
      ref = prev;
      i--;
    }
  }else{
    while(n < i){
      prev = ref->prev;
      self->Alloc->free(self->alloc, ref);
      ref = prev;
      i--;
    }
  }
  __link_nodes(prev, self->end);
  return self;
}

static list_t*
_merge(list_t* self, list_t* other, cmp_f* lt)
{
  list_ref_t a, b;

  assert(self);
  assert(other);
  assert(lt);
  
  if( self == other )
    return self;

  a = self->bbeg->next;
  b = other->bbeg->next;

  while( a != self->end && b != other->end ){
    if( lt(a->value, b->value) ){
      /* a < b, advance a, and splice [start, b) before a  */
      if( other->bbeg->next != b ){
        __link_nodes(a->prev, other->bbeg->next);
        __link_nodes(b->prev, a);
        __link_nodes(other->bbeg, b);
      }
      a = a->next;
    } else { /*  a >= b, advance b */
      b = b->next;
    }
  }

  /* splice `other` before a
   * 1. a == end --> `other` >= `self`.
   * 2. b == end --> `other` < `self`.
   */
  if( ! _empty(other) ){
    __link_nodes(a->prev, other->bbeg->next);
    __link_nodes(other->end->prev, a);
    __link_nodes(other->bbeg, other->end);
  }

  return self;
}

static list_t*
_splice(list_t* self, list_ref_t pos, list_t* other)
{
  assert(self);
  assert(other);

  /* FIXME: assert( self->Object == other->Object ) */

  if( self == other || _empty(other))
    return self;

  __link_nodes(pos->prev, other->bbeg->next);
  __link_nodes(other->end->prev, pos);
  __link_nodes(other->bbeg, other->end);
  return self;
}

static list_t*
_splice_at(list_t* self, list_ref_t pos, list_t* other, list_ref_t it)
{
  assert(self);
  assert(other);

  if( pos == it || it == other->bbeg )
    return self;

  assert( it != other->end );

  __link_nodes(it->prev, it->next);
  __link_nodes(pos->prev, it);
  __link_nodes(it, pos);
  return self;
}

static list_t*
_splice_r(list_t* self, list_ref_t pos, list_t* other, list_ref_t first, list_ref_t last)
{
  list_ref_t prev;

  assert(self);
  assert(other);

  if(pos == first || first == last || first == other->bbeg)
    return self;

  assert(first != other->end);

  prev = first->prev;
  __link_nodes(pos->prev, first);
  __link_nodes(last->prev, pos);
  __link_nodes(prev, last);
  return self;
}


static list_t*
_remove(list_t* self, cmp_f* pred, const object_t* value)
{
  list_ref_t ref, next;

  assert(self);
  assert(pred);

  ref = self->bbeg->next;
  if(self->Object->_del_){
    while( ref != self->end ){
      next = ref->next;
      if( pred(ref->value, value) ){
        __link_nodes(ref->prev, ref->next);
        self->Object->_del_(ref->value);
        self->Alloc->free(self->alloc, ref);
      }
      ref = next;
    }
  }else{
    while( ref != self->end ){
      next = ref->next;
      if( pred(ref->value, value) ){
        __link_nodes(ref->prev, ref->next);
        self->Alloc->free(self->alloc, ref);
      }
      ref = next;
    }
  }
  return self;
}

static list_t*
_reverse(list_t* self)
{
  assert(self);
  __reverse_nodes(self->bbeg, self->end);
  return self;
}

static list_t*
_unique(list_t* self, cmp_f* pred)
{
  list_ref_t first, next;
  assert(self);
  assert(pred);

  first = self->bbeg->next;

  if(first != self->end){ /* size >= 1 */
    assert(first);
    next = first->next;
    if(self->Object->_del_){
      while(next != self->end){
        if( ! pred(first->value, next->value) ){
          first = next;
          next = next->next;
        }else{
          __link_nodes(first, next->next);
          self->Object->_del_(next->value);
          self->Alloc->free(self->alloc, next);
          next = first->next;
        }
      } /* ~ while */
    }else{
      while(next != self->end){
        if( ! pred(first->value, next->value) ){
          first = next;
          next = next->next;
        }else{
          __link_nodes(first, next->next);
          self->Alloc->free(self->alloc, next);
          next = first->next;
        }
      } /* ~ while */
    }
  }
  return self;
}

static list_t*
_sort(list_t* self, cmp_f* lt)
{
#undef  N
#define N (8 * sizeof(size_t)) /* 32-bit: 32, 64-bit: 64 */

  if( _size(self) <= 1 )
    return self;

  {
    size_t i;
    list_t carry, tmp[N];
    list_t *fill = &tmp[0], *counter;
 
    assert(self);
    assert(lt);
 
    _init_(&carry, self->Object);
    for(i=0; i<N; i++)
      _init_(&tmp[i], self->Object);
 
    do{
      _splice_at(&carry, _begin(&carry), self, _begin(self));

      for( counter = &tmp[0]; counter != fill && ! _empty(counter); ++counter){
        _merge(counter, &carry, lt);
        _swap(&carry, counter);
      }
      _swap(&carry, counter);
      if( counter == fill )
        ++ fill;

    }while( ! _empty(self) );

    for( counter = &tmp[1]; counter != fill; ++counter )
      _merge(counter, counter - 1, lt);

    _swap(self, fill - 1);

    _del_(&carry);
    for(i=0; i<N; i++)
      _del_(&tmp[i]);
  }
  return self;
#undef N
}

const list_z List = 
{
  ._sizeof_ = sizeof(list_t),
  ._init_   = _init_,
  ._initcp_ = _initcp_,
  ._del_    = _del_,
  .get      = _get,
  .set      = _set,
  .front    = _front,
  .back     = _back,
  .begin    = _begin,
  .end      = _end,
  .rbegin   = _rbegin,
  .rend     = _rend,
  .find     = _find,
  .empty    = _empty,
  .size     = _size,
  .clear    = _clear,
  .insert   = _insert,
  .insert_n = _insert_n,
  .insert_r = _insert_r,
  .erase    = _erase,
  .erase_r  = _erase_r,
  .push_back  = _push_back,
  .pop_back   = _pop_back,
  .push_front = _push_front,
  .pop_front  = _pop_front,
  .resize     = _resize,
  .merge      = _merge,
  .splice     = _splice,
  .splice_at  = _splice_at,
  .splice_r   = _splice_r,
  .remove     = _remove,
  .reverse    = _reverse,
  .unique     = _unique,
  .sort       = _sort,
  .swap       = _swap,
};


/* - list IO - */
static list_io_t*
_io_init_(list_io_t* self, stdio_t* sub, const io_z* SSub)
{
  assert(self);
  assert(sub);

  self->std = sub;
  self->SSub = SSub;
  return self;
}

static list_io_t*
_io_initcp_(list_io_t* self, const list_io_t* other)
{
  assert(other);
  assert(self);
  assert(other->std);

  *self = *other;
  return self;
}

static int 
_io_read(list_t* out, list_io_t* in, const io_z* Sub)
{
  /* list: [*,*,*,*] */
  int c;
  list_ref_t ref, ref0;
  size_t i = 0;

  assert(out);
  assert(in);
  assert(Sub);

#undef  __skip
#define __skip  \
  while( true ){ \
    if( (c = fgetc(in->std)) == EOF ){ \
      i = EOF; \
      goto done; \
    } else if( c == ',' ){ \
      NULL; \
    } else if( c == ']' ){ /* end of list */ \
      goto done; \
    } else if( c == '[' ){ /* beg of list */ \
      ref = out->bbeg->next; \
      i = 0; \
    } else { \
      if( ungetc(c, in->std) == EOF){ i = EOF; goto done; } else break; \
    } \
  }

  ref = out->bbeg->next;

  while( true ){
    if( (c = fgetc(in->std)) == EOF ){
      i = EOF;
      goto done;
    } else if( c == '[' ){ /* beg of list. */
      ref = out->bbeg->next;
      i = 0;
      while(true){ /* scan content */
        if( ref == out->end ){ /* insert new node */
          ref0 = ref;
          ref = out->Alloc->malloc(out->alloc, __node_size(out));
          if( ref == NULL ){
            fprintf(stderr, "ListIO.read() malloc failed.\n");
            return i;
          }
          if( (c = Sub->read(ref->value, in->std, in->SSub)) == EOF ){
            out->Alloc->free(out->alloc, ref);
            ref = ref0;
            i = EOF;
            goto done;
          } else if ( c > 0 ){
            __insert_before(out->end, ref);
            i++;
            ref = ref->next;
            __skip;
          } else {
            __skip;
          }
        } else { /* overwrite node */
          if( ( c = Sub->read(ref->value, in->std, in->SSub)) == EOF ) {
            i = EOF;
            goto done;
          }else if ( c > 0 ){
            ref = ref->next;
            i++;
            __skip;
          } else {
            __skip;
          }
        }
      }
    } else if( c == ']' ){ /* end of list */
      break;
    }
  }
done:
  __link_nodes(ref->prev, out->end);
  __free_nodes(out, ref, out->end);
  return i;
#undef __skip
}

static int
_io_write(const list_t* in, list_io_t* out, const io_z* Sub)
{
  /* list: [*,*,*,*] */
  list_ref_t ref;
  size_t i = 0;
  int c;

  assert(in);
  assert(out);
  assert(Sub);

#undef  __fputc
#define __fputc(chr, out) { if(fputc(chr, out) == EOF) return EOF; }

#undef  __write
#define __write \
  { \
    if( (c = Sub->write(ref->value, out->std, out->SSub)) == EOF ){ \
      return EOF; \
    } else if( c == 0 ){ \
      return i; \
    } \
  }

  __fputc('[', out->std); /* beg */

  ref = in->bbeg->next;
  if( ref != in->end ){
    __write;
    ref = ref->next; 
    i++; 
  }
  while( ref != in->end ){
    __fputc(',', out->std); /* sep */
    __write;
    ref = ref->next; 
    i++; 
  }

  __fputc(']', out->std); /* end */
  fflush(out->std);
  return i;
#undef  __fputc
#undef  __write
}

const list_io_z ListIO =
{
  ._sizeof_ = sizeof(list_io_t),
  ._init_   = _io_init_,
  ._initcp_ = _io_initcp_,
  ._del_    = NULL,
  .read     = _io_read,
  .write    = _io_write,
};


