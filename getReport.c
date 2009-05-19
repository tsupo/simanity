/*
 *  �u���܂ɂĂ��v�́u�����̂��܁v�̌������擾����
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/getReport.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) �r���h���̃f�B���N�g���\���𐮗�
 * (2) VSS�T�[�o���_��ύX
 * 
 * 1     06/09/15 16:51 Tsujimura543
 * getReport() �� simanity.c ���番�����AgetReport.c ���쐬
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/getReport.c 1     09/05/14 4:45 tsupo $";
#endif

char    *
getReport(
        const char *dst,        /* (I)   ���܏��A�N�Z�X URL �̑��΃p�X    */
        char       *cookie,     /* (I/O) �N�b�L�[                           */
        char       *pass,       /* (O)   ���܏��A�N�Z�X�ɕK�v�ȃp�X���[�h */
        char       *response,   /* (I/O) ��M�d���i�[�̈�                   */
        size_t     sz           /* (I)   ��M�d���i�[�̈�T�C�Y             */
    )
{
    char        url[MAX_URLLENGTH];
    const char  *p, *q;

    pass[0] = NUL;
    p = strstr( dst, "password=" );
    if ( p ) {
        p += 9;
        q = strchr( p, '&' );
        if ( q ) {
            strncpy( pass, p, q - p );
            pass[q - p] = NUL;
        }
    }

    sprintf( url, "http://g.sima.jp%s", dst );
    setUpReceiveBuffer( response, sz );
    http_getEx( url, response, cookie );

    return ( response );
}
