
/*
 * $RCSfile$
 *--------------------------------------------------------------------
 * DESCRIPTION
 *   writes a PicFile to disk
 *
 * $Log$
 * Revision 1.9  1999/11/27 19:40:44  andre
 * *** empty log message ***
 *
 * Revision 1.8  1999/11/27  19:24:19  andre
 * *** empty log message ***
 *
 * Revision 1.3.2.3  1998/03/25  15:03:36  andre
 * added info->pixel_start_in_file
 *
 * Revision 1.3.2.2  1997/10/09  11:03:53  andre
 * *** empty log message ***
 *
 * Revision 1.3.2.1  1997/09/15  13:46:15  andre
 * added encryption
 *
 * Revision 1.3  1997/09/15  13:21:18  andre
 * switched to new what string format
 *
 * Revision 1.2  1997/09/15  10:24:13  andre
 * ipPicPut now returns status
 *
 * Revision 0.1  1993/04/02  16:18:39  andre
 * now works on PC, SUN and DECstation
 *
 * Revision 0.0  1993/03/26  12:56:26  andre
 * Initial revision, NO error checking
 *
 *
 *--------------------------------------------------------------------
 *  COPYRIGHT (c) 1993 by DKFZ (Dept. MBI) Heidelberg, FRG
 */
#ifndef lint
  static char *what = { "@(#)ipPicPut\t\tDKFZ (Dept. MBI)\t"__DATE__"\t$Revision$" };
#endif

#include "ipPic.h"

int
ipPicPut( char *outfile_name, ipPicDescriptor *pic )
{
  FILE *outfile;

  ipUInt4_t len;
  ipUInt4_t tags_len;

  if( pic->info->write_protect )
    {
      fprintf( stderr, "ipPicPut: sorry, can't write (missing tags !!!)\n" );
      return( -1 );
    }

  if( ipPicEncryptionType(pic) != ' ' )
    {
      fprintf( stderr, "ipPicPut: sorry, can't write (was encrypted !!!)\n" );
      return( -1 );
    } 

  if( outfile_name == NULL )
    outfile = stdout;
  else if( strcmp(outfile_name, "stdout") == 0 )
    outfile = stdout;
  else
    outfile = fopen( outfile_name, "wb" );

  if( outfile == NULL )
    {
      fprintf( stderr, "ipPicPut: sorry, error opening outfile\n" );
      return( -1 );
    }

  tags_len = _ipPicTagsSize( pic->info->tags_head );

  len = tags_len +        3 * sizeof(ipUInt4_t)
                 + pic->dim * sizeof(ipUInt4_t);

  /* write oufile */
  if( ipPicEncryptionType(pic) == ' ' )
    fwrite( ipPicVERSION, 1, sizeof(ipPicTag_t), outfile );
  else
    fwrite( pic->info->version, 1, sizeof(ipPicTag_t), outfile );

  ipFWriteLE( &len, sizeof(ipUInt4_t), 1, outfile );

  ipFWriteLE( &(pic->type), sizeof(ipUInt4_t), 1, outfile );
  ipFWriteLE( &(pic->bpe), sizeof(ipUInt4_t), 1, outfile );
  ipFWriteLE( &(pic->dim), sizeof(ipUInt4_t), 1, outfile );

  ipFWriteLE( pic->n, sizeof(ipUInt4_t), pic->dim, outfile );

  _ipPicWriteTags( pic->info->tags_head, outfile, ipPicEncryptionType(pic) );

  pic->info->pixel_start_in_file = ftell( outfile );

  if( pic->data )
    {
      if( pic->type == ipPicNonUniform )
        fwrite( pic->data, pic->bpe / 8, _ipPicElements(pic), outfile );
      else
        ipFWriteLE( pic->data, pic->bpe / 8, _ipPicElements(pic), outfile );
    }

  if( outfile != stdout )
    fclose( outfile );

  return( 0 );
}

ipUInt4_t _ipPicTagsSize( _ipPicTagsElement_t *head )
{
  _ipPicTagsElement_t *current = head;
  ipUInt4_t tags_len;

  tags_len = 0;
  while( current != NULL )
    {
      ipUInt4_t  len;

      if( current->tsv->type == ipPicTSV )
        {
          if( current->tsv->dim == 0 )
            {
              current->tsv->n[0] = _ipPicTagsNumber(current->tsv->value);

              if( current->tsv->n[0] > 0 )
                current->tsv->dim = 1;
            }

          len = _ipPicTagsSize( (_ipPicTagsElement_t *)current->tsv->value );
        }
      else
        {
          ipUInt4_t  elements;

          elements = _ipPicTSVElements( current->tsv );

          if( current->tsv->type == ipPicASCII
              || current->tsv->type == ipPicNonUniform )
            current->tsv->bpe = 8;

          len = elements * current->tsv->bpe / 8;
        }

      tags_len += 32                                       /* name           */
                  +                     sizeof( ipUInt4_t) /* len            */
                  +                 3 * sizeof( ipUInt4_t) /* type, bpe, dim */
                  + current->tsv->dim * sizeof( ipUInt4_t) /* n[]            */
                  + len;                                   /* data           */

      current = current->next;
    }
  return( tags_len );
}

ipUInt4_t _ipPicTagsNumber( _ipPicTagsElement_t *head )
{
  _ipPicTagsElement_t *current = head;
  ipUInt4_t tags_number;

  tags_number = 0;
  while( current != NULL )
    {
      tags_number++;

      current = current->next;
    }
  return( tags_number );
}

void _ipPicWriteTags( _ipPicTagsElement_t *head, FILE *stream, char encryption_type )
{
  _ipPicTagsElement_t *current = head;

  while( current != NULL )
    {
      ipUInt4_t  elements;
      ipUInt4_t  len;

      elements = _ipPicTSVElements( current->tsv );

      if( current->tsv->type == ipPicTSV )
        {
          if( current->tsv->dim == 0 )
            {
              current->tsv->n[0] = _ipPicTagsNumber(current->tsv->value);

              if( current->tsv->n[0] > 0 )
                current->tsv->dim = 1;
            }

          assert( elements == _ipPicTagsNumber(current->tsv->value) );

          len = _ipPicTagsSize( current->tsv->value );
        }
      else
        {
          if( current->tsv->type == ipPicASCII
              || current->tsv->type == ipPicNonUniform )
            current->tsv->bpe = 8;

          len = elements * current->tsv->bpe / 8;

         }

      len +=                   3 * sizeof(ipUInt4_t)  /* type, bpe, dim */
             + current->tsv->dim * sizeof(ipUInt4_t); /* n[]            */

      fwrite( current->tsv->tag, 1, sizeof(ipPicTag_t), stream );

      ipFWriteLE( &len, sizeof(ipUInt4_t), 1, stream );

      ipFWriteLE( &(current->tsv->type), sizeof(ipUInt4_t), 1, stream );
      ipFWriteLE( &(current->tsv->bpe), sizeof(ipUInt4_t), 1, stream );
      ipFWriteLE( &(current->tsv->dim), sizeof(ipUInt4_t), 1, stream );
      ipFWriteLE( &(current->tsv->n), sizeof(ipUInt4_t), current->tsv->dim, stream );

      if( current->tsv->type == ipPicTSV )
        {
          _ipPicWriteTags( current->tsv->value,
                           stream,
                           encryption_type );
        }
      else
        {
          ipFWriteLE( current->tsv->value, current->tsv->bpe / 8, elements, stream );
        }

      current = current->next;
    }
}
