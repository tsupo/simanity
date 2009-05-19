/*
 *  �u���܂ɂĂ��v�́u���܂�����v�ɑ��������A�� http ���N�G�X�g�����s����
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/visit.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) �r���h���̃f�B���N�g���\���𐮗�
 * (2) VSS�T�[�o���_��ύX
 * 
 * 2     07/03/29 18:39 Tsujimura543
 * 2007�N3��27���́u���܂ɂĂ��v�̃����e�i���X�Ŏd�l�ɕύX��������������
 * �Ή����邽�߂̏C�������{
 * 
 * 1     06/09/15 16:03 Tsujimura543
 * visitSima(), getArea(), getMapURL() �� simanity.c ���番�����Avisit.c
 * ���쐬
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/visit.c 1     09/05/14 4:45 tsupo $";
#endif


/* ��: ���M�d���i�[�̈�A��M�d���i�[�̈�́A�ȉ��̊e�֐��Ŏg���܂킷���Ƃ� */
/*     �z�肵�Ă��� (�̈�͊e�֐��̌Ăяo�����ł��炩���ߊm�ۂ��Ă�������)  */

void
visitSima(
        const char *simaID,     /* (I)   ���܂ɂĂ�ID (���[�UID)            */
        const char *hash,       /* (I)   ���܂ɂĂ�ID�ɕR�t����ꂽ�n�b�V�� */
        char       *cookie,     /* (I/O) �N�b�L�[                           */
        char       *src,        /* (O)   ���ɃA�N�Z�X���ׂ� URL             */
        char       *request,    /* (I/O) ���M�d���i�[�̈�                   */
        char       *response,   /* (I/O) ��M�d���i�[�̈�                   */
        size_t     sz           /* (I)   ��M�d���i�[�̈�T�C�Y             */
    )
{
    char    url[MAX_URLLENGTH];

    setUpReceiveBuffer( response, sz );
    sprintf( request,
             "p=h_home&"
             "c_member_id=%s&"
             "user_hash=%s&"
             "game_type=0",
             simaID,    // ���܂ɂĂ�ID
             hash       // �n�b�V��
           );
    strcpy( url, "http://g.sima.jp/page.php" );
    http_postEx( url, "application/x-www-form-urlencoded",
                 request, response, cookie );
    if ( *response ) {
        const char  *p, *q, *r;

        p = response;
        q = strstr( p, "<iframe src=\"" );
        if ( q ) {
            q += 13;
            r = strchr( q, '"' );
            if ( r ) {
                strncpy( src, q, r - q );
                src[r - q] = NUL;
            }
        }
    }
    /* �������� http://g.sima.jp/* �ɃA�N�Z�X������́Ahash �ɑ��������� */
    /* �� cookie �Ɋi�[�����̂ŁA�ȍ~�̃A�N�Z�X�ł� hash �͕s�v�ɂȂ�    */
}

void
getArea(
        const char *src,        /* (I) visitSima() �Ŏ擾���� URL           */
        char       *cookie,     /* (I/O) �N�b�L�[                           */
        char       *area,       /* (O)   �C�於                             */
                                /*         �u���ɃA�N�Z�X���ׂ� URL�v���m�� */
                                /*         ����̂ɕK�v                     */
        char       *response,   /* (I/O) ��M�d���i�[�̈�                   */
        size_t     sz           /* (I)   ��M�d���i�[�̈�T�C�Y             */
    )
{
    setUpReceiveBuffer( response, sz );
    http_getEx( src, response, cookie );
    if ( *response ) {
        const char  *p, *q, *r;

        p = response;
        q = strstr( p, " name='area' value='" );
        if ( q ) {
            q += 20;
            r = strchr( q, '\'' );
            if ( r ) {
                strncpy( area, q, r - q );
                area[r - q] = NUL;
            }
        }
    }
}

void
getMapURL(
        const char *area,     /* (I)   getArea() �Ŏ擾�����u�C�於�v       */
        char       *cookie,   /* (I/O) �N�b�L�[                             */
        char       *dst,      /* (O)   �������𓾂邽�߂ɃA�N�Z�X���� URL */
        char       *request,  /* (I/O) ���M�d���i�[�̈�                     */
        char       *response, /* (I/O) ��M�d���i�[�̈�                     */
        size_t     sz         /* (I)   ��M�d���i�[�̈�T�C�Y               */
    )
{
    char    url[MAX_URLLENGTH];

    strcpy( url, "http://g.sima.jp/cgi-bin/n/island.cgi" );
    sprintf( request,
             "s=r&"
             "area=%s&"
             "framemode=outer",
             area );
    setUpReceiveBuffer( response, sz );
    http_postEx( url, "application/x-www-form-urlencoded",
                 request, response, cookie );
    if ( *response ) {
        const char  *p, *q, *r;

        p = response;
        q = strstr( p, "CONTENT='1;URL=" );
        if ( q ) {
            q += 15;
            r = strchr( q, '\'' );
            if ( r ) {
                strncpy( dst, q, r - q );
                dst[r - q] = NUL;
            }
        }
    }
}

