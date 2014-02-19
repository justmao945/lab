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
#include <yacl/slist.h>

/*
 * links two nodes: lhs --> rhs.
 * @param lhs, rhs (slist_ref_t)
 * @return lhs
 */
#undef  __link_nodes
#define __link_nodes(lhs, rhs) \
  ( (lhs)->next = (rhs) )


/* 
 * Inserts `node` after `pos`.
 * @param pos, node (slist_ref_t)
 * @return
 */
#undef  __insert_after
#define __insert_after(pos, node) \
  ( __link_nodes(node, (pos)->next), __link_nodes(pos, node) )

/*
 * Return node byte size.
 * @param slist (slist_t*)
 */
#undef  __node_size
#define __node_size(slist) ((slist)->Object->_sizeof_ + sizeof(struct __slist_ref_t))

/**
 * Advance the node by `n` elements.
 * Return advanced node.
 */
static slist_ref_t
__advance_node(slist_ref_t node, ptrdiff_t n)
{
  while(n-- > 0 && node != NULL)
    node = node->next;
  return node;
}

/*
 * Counts the number of elements between [first, last).
 */
static size_t
__count_nodes(slist_ref_t first, slist_ref_t last)
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
static slist_ref_t
__reverse_nodes(slist_ref_t before_first, slist_ref_t last)
{
  register slist_ref_t first, next, prev;

  if( __count_nodes(before_first, last) <= 2 )
    return before_first;

  prev = before_first->next;
  first = prev->next;
  prev->next = last; /* reverse the first node */
  next = first->next;
  while(next != last && next != NULL){
    first->next = prev;
    prev = first;
    first = next;
    next = next->next;
  }
  first->next = prev;
  __link_nodes(before_first, first);
  return before_first;
}


/*
 * Frees nodes between `(before_first, last)`.
 */
static void
__free_nodes(slist_t* slist, slist_ref_t before_first, slist_ref_t last)
{
  register slist_ref_t next, first;

  if( __count_nodes(before_first, last) <= 1 )
    return;

  first = before_first->next;
  next  = first->next;

  if(slist->Object->_del_){
    while(next != last && next != NULL){
      slist->Object->_del_(first->value);
      slist->Alloc->free(slist->alloc, first);
      first = next;
      next = next->next;
    }
    slist->Object->_del_(first->value);
  }else{
    while(next != last && next != NULL){
      slist->Alloc->free(slist->alloc, first);
      first = next;
      next = next->next;
    }
  }
  slist->Alloc->free(slist->alloc, first);
  __link_nodes(before_first, last);
}


/* slist reference */
static slist_ref_t*
_ref_initcp_(slist_ref_t *self, const slist_ref_t *other)
{
  assert(self);
  assert(other);
  *self = *other;
  return self;
}

static object_t*
_ref_value(slist_ref_t ref)
{
  return (object_t*)ref->value;
}

static slist_ref_t
_ref_next(slist_ref_t ref)
{
  assert(ref);
  return ref->next;
}

static slist_ref_t
_ref_next_n(slist_ref_t ref, ptrdiff_t n)
{
  return __advance_node(ref, n);
}

static ptrdiff_t
_ref_dist(slist_ref_t first, slist_ref_t last)
{
  return __count_nodes(first, last);
}

const slist_ref_z SListRef = 
{
  ._sizeof_ = sizeof(slist_ref_t),
  ._init_   = NULL,
  ._initcp_ = _ref_initcp_,
  ._del_    = NULL,
  .value    = _ref_value,
  .next     = _ref_next,
  .next_n   = _ref_next_n,
  .dist     = _ref_dist,
};

/* -- slist -- */

static slist_t*
_init_(slist_t* self, const object_z* Object)
{
  assert(self);
  assert(Object);
  assert(Object->_sizeof_);
  assert(Object->_initcp_);

  self->Object = Object;
  self->alloc  = &_alloc_;
  self->Alloc  = &Alloc;

  /* FIXME: Use Alloc->malloc ? */
  self->bbeg = (slist_ref_t)malloc(__node_size(self));
  if( self->bbeg == NULL ){
    fprintf(stderr, "SList._init_(): malloc failed\n");
    return self;
  }
  __link_nodes(self->bbeg, NULL); /* empty slist */
  return self;
}

static slist_t*
_swap(slist_t* self, slist_t* other)
{
  slist_t t;
  assert(self);
  assert(other);
  t = *self;
  *self = *other;
  *other = t;
  return self;
}

static slist_t* 
_del_(slist_t* self)
{
  assert(self);
  __free_nodes(self, self->bbeg, NULL);
  free(self->bbeg);
  return (slist_t*)memset(self, 0, sizeof(slist_t));
}

static slist_ref_t
_insert_after(slist_t* self, slist_ref_t pos, const object_t* value)
{
  slist_ref_t ref;

  assert(self);
  assert(pos);
  assert(value);

  ref = (slist_ref_t)self->Alloc->malloc(self->alloc, __node_size(self));
  if ( ref == NULL ){
    fprintf(stderr, "SList.insert_after():malloc failed.\n");
    return pos;
  }
  self->Object->_initcp_(ref->value, value);
  __insert_after(pos, ref);
  return ref;
}

static slist_t*
_initcp_(slist_t* self, const slist_t* other)
{
  slist_ref_t ref, pos;

  assert(self);
  assert(other);

  self->Object = other->Object;
  self->Alloc = other->Alloc;
  self->alloc = other->alloc;

  /* FIXME: Use Alloc->malloc ? */
  self->bbeg = (slist_ref_t)malloc(__node_size(self));
  if( self->bbeg == NULL ){
    fprintf(stderr, "SList._initcp_(): malloc failed\n");
    return self;
  }
  __link_nodes(self->bbeg, NULL); /* empty slist */

  ref = other->bbeg->next;
  pos = self->bbeg;
  while( ref != NULL ){
    pos = _insert_after(self, pos, ref->value);
    ref = ref->next;
  }
  return self;
}

static object_t*
_get(const slist_t* self, size_t pos)
{
  slist_ref_t ref;
  assert(self);

  ref = self->bbeg->next;
  while( ref != NULL && pos-- > 0 )
    ref = ref->next;
  return ref == NULL ? NULL : (object_t*)ref->value;
}

static slist_t*
_set(slist_t* self, size_t pos, const object_t* value)
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
_empty(const slist_t* self)
{
  assert(self);
  return self->bbeg->next == NULL;
}

static object_t*
_front(const slist_t* self)
{
  assert(self);
  assert( ! _empty(self) );
  return self->bbeg->next->value;
}

static slist_ref_t
_before_begin(const slist_t* self)
{
  assert(self);
  return self->bbeg;
}

static slist_ref_t
_begin(const slist_t* self)
{
  assert(self);
  return self->bbeg->next;
}

static slist_ref_t
_end(const slist_t* self)
{
  return NULL;
}

static size_t
_size(const slist_t* self)
{
  assert(self);
  return __count_nodes(self->bbeg->next, NULL);
}

static slist_t*
_clear(slist_t* self)
{
  assert(self);
  __free_nodes(self, self->bbeg, NULL);
  __link_nodes(self->bbeg, NULL);
  return self;
}

static slist_ref_t
_insert_after_n(slist_t* self, slist_ref_t pos, size_t n, const object_t* data)
{
  assert(self);
  assert(pos);

  while (n > 0){
    pos = _insert_after(self, pos, data);
    data += self->Object->_sizeof_;
    n--;
  }
  return pos;
}

static slist_ref_t
_insert_after_r(slist_t* self, slist_ref_t pos,
    const ref_z* Ref, ref_t first, ref_t last)
{
  assert(self);
  assert(pos);
  assert(Ref);

  while( first != last ){
    pos = _insert_after(self, pos, Ref->value(first));
    first = Ref->next(first);
  }
  return pos;
}

static slist_ref_t
_erase_after(slist_t* self, slist_ref_t pos)
{
  assert(self);
  assert(pos);

  if( pos != NULL && pos->next != NULL ){
    slist_ref_t ret = pos->next->next;
    __link_nodes(pos, ret);
    if(self->Object->_del_) self->Object->_del_(pos->next->value);
    self->Alloc->free(self->alloc, pos->next);
    return ret;
  } else {
    return pos;
  }
}

static slist_ref_t
_erase_after_r(slist_t* self, slist_ref_t first, slist_ref_t last)
{
  assert(self);
  assert(first != NULL && first->next != NULL);
  __free_nodes(self, first, last);
  __link_nodes(first, last);
  return last;
}

static slist_t*
_push_front(slist_t* self, const object_t* value)
{
  slist_ref_t ref;

  assert(self);
  assert(value);
  
  ref = (slist_ref_t)self->Alloc->malloc(self->alloc, __node_size(self));
  if( ref == NULL ){
    fprintf(stderr,  "SList.push_front(): malloc failed.\n");
    return self;
  }
  self->Object->_initcp_(ref->value, value);
  __insert_after(self->bbeg, ref);
  return self;
}

static slist_t*
_pop_front(slist_t* self)
{
  slist_ref_t front;

  assert(self);
  assert( ! _empty(self) );

  front = self->bbeg->next;
  __link_nodes(self->bbeg, front->next);
  if(self->Object->_del_) self->Object->_del_(front->value);
  self->Alloc->free(self->alloc, front);
  return self;
}

static slist_t*
_resize(slist_t* self, size_t n, const object_t* value)
{
  size_t size;
  slist_ref_t before_last;

  assert(self);
  before_last = self->bbeg;
  size = 0;
  while( before_last->next != NULL && size < n ){
    before_last = before_last->next;
    size++;
  }

  while( size < n ){
    before_last = _insert_after(self, before_last, value);
    size++;
  }
  __free_nodes(self, before_last, NULL);
  __link_nodes(before_last, NULL);
  return self;
}

static slist_t*
_merge(slist_t* self, slist_t* other, cmp_f* lt)
{
  slist_ref_t a, b, a_next, b_next;

  assert(self);
  assert(other);
  assert(lt);
  
  if( self == other )
    return self;

  a = self->bbeg;
  b = other->bbeg;
  a_next = a->next;
  b_next = b->next;

  while( a_next != NULL && b_next != NULL ){
    if( lt(a_next->value, b_next->value) ){
      /* a->next < b->next, advance a, and splice [start, b] after a */
      if( other->bbeg->next != b_next ){
        __link_nodes(a, other->bbeg->next);
        __link_nodes(other->bbeg, b_next);
        __link_nodes(b, a_next);
      }
      a = a_next;
      a_next = a_next->next;
    } else { /*  a >= b, advance b */
      b = b_next;
      b_next = b_next->next;
    }
  }
  /* splice `other` after a
   * 1. a->next == end --> `other` >= `self`.
   * 2. b->next == end --> `other` < `self`.
   */
  if( ! _empty(other) ){
    if( a->next != NULL ){
      slist_ref_t b_end;

      b_end = other->bbeg;
      while( b_end->next != NULL )
        b_end = b_end->next;

      __link_nodes(b_end, a->next);
    }
    __link_nodes(a, other->bbeg->next);
    __link_nodes(other->bbeg, NULL);
  }

  return self;
}

static slist_t*
_splice_after(slist_t* self, slist_ref_t pos, slist_t* other)
{
  assert(self);
  assert(other);

  /* FIXME: assert( self->Object == other->Object ) */

  if( self == other || _empty(other) || pos == NULL)
    return self;
  
  if( pos->next != NULL ){
    slist_ref_t b_end = other->bbeg;
    while( b_end->next != NULL ) b_end = b_end->next;
    __link_nodes(b_end, pos->next);
  }
  __link_nodes(pos, other->bbeg->next);
  __link_nodes(other->bbeg, NULL);
  return self;
}

static slist_t*
_splice_after_at(slist_t* self, slist_ref_t pos, slist_t* other, slist_ref_t it)
{
  slist_ref_t next;
  assert(self);
  assert(other);

  if( pos == it || pos == NULL || it == NULL || it->next == NULL )
    return self;

  next = it->next->next;
  __link_nodes( it->next, pos->next );
  __link_nodes( pos, it->next );
  __link_nodes( it, next );

  return self;
}

/* (first, last) --> after pos */
static slist_t*
_splice_after_r(slist_t* self, slist_ref_t pos,
    slist_t* other, slist_ref_t first, slist_ref_t last)
{
  assert(self);
  assert(other);

  if(pos == first || first == last || pos == NULL ||
      first == NULL || first->next == NULL)
    return self;

  if( pos->next != NULL ){
    slist_ref_t b_last = first;
    while( b_last->next != NULL )  b_last = b_last ->next;
    __link_nodes(b_last, pos->next);
  }
  __link_nodes(pos, first->next);
  __link_nodes(first, last);
  return self;
}

static slist_t*
_remove(slist_t* self, cmp_f* pred, const object_t* value)
{
  slist_ref_t ref, next;

  assert(self);
  assert(pred);

  ref = self->bbeg;
  if(self->Object->_del_){
    while( ref->next != NULL ){
      if( pred(ref->next->value, value) ){
        next = ref->next->next;
        self->Object->_del_(ref->next->value);
        self->Alloc->free(self->alloc, ref->next);
        __link_nodes(ref, next);
      }
      ref = ref->next;
    }
  }else{
    while( ref->next != NULL ){
      if( pred(ref->next->value, value) ){
        next = ref->next->next;
        self->Alloc->free(self->alloc, ref->next);
        __link_nodes(ref, next);
      }
      ref = ref->next;
    }
  }
  return self;
}

static slist_t*
_reverse(slist_t* self)
{
  assert(self);
  __reverse_nodes(self->bbeg, NULL);
  return self;
}

static slist_t*
_unique(slist_t* self, cmp_f* pred)
{
  slist_ref_t first, next;
  assert(self);
  assert(pred);

  first = self->bbeg->next;

  if(first != NULL){ /* size >= 1 */
    next = first->next;
    if(self->Object->_del_){
      while(next != NULL){
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
      while(next != NULL){
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

static slist_t*
_sort(slist_t* self, cmp_f* lt)
{
#undef  N
#define N (8 * sizeof(size_t)) /* 32-bit: 32, 64-bit: 64 */

  if( _size(self) <= 1 )
    return self;

  {
    size_t i;
    slist_t carry, tmp[N];
    slist_t *fill = &tmp[0], *counter;
 
    assert(self);
    assert(lt);
 
    _init_(&carry, self->Object);
    for(i=0; i<N; i++)
      _init_(&tmp[i], self->Object);
 
    do{
      _splice_after_at(&carry, _before_begin(&carry), self, _before_begin(self));

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

static slist_ref_t 
_find_next(const slist_t* self, cmp_f *eq, const object_t* data)
{
  slist_ref_t bbeg;
  assert(self);
  assert(eq);
  
  bbeg = self->bbeg;
  while( bbeg->next != NULL ){
    if(  eq(bbeg->next->value, data) )
      break;
    bbeg = bbeg->next;
  }
  return bbeg;
}


const slist_z SList = 
{
  ._sizeof_ = sizeof(slist_t),
  ._init_   = _init_,
  ._initcp_ = _initcp_,
  ._del_    = _del_,
  .get      = _get,
  .set      = _set,
  .front    = _front,
  .begin    = _begin,
  .end      = _end,
  .empty    = _empty,
  .size     = _size,
  .clear    = _clear,
  .push_front = _push_front,
  .pop_front  = _pop_front,
  .remove     = _remove,
  .resize     = _resize,
  .merge      = _merge,
  .reverse    = _reverse,
  .unique     = _unique,
  .sort       = _sort,
  .swap       = _swap,
  .find_next  = _find_next,
  .before_begin   = _before_begin,
  .insert_after   = _insert_after,
  .insert_after_n = _insert_after_n,
  .insert_after_r = _insert_after_r,
  .erase_after    = _erase_after,
  .erase_after_r  = _erase_after_r,
  .splice_after     = _splice_after,
  .splice_after_at  = _splice_after_at,
  .splice_after_r   = _splice_after_r,
};

