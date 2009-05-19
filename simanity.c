/*
 *  �u���܂ɂĂ��v�̎����̓��̌��݂̏󋵂��擾�A�\������
 *          wriietn by H.Tsujimura  30 Aug 2006
 *
 * History:
 * $Log: /comm/simanity/simanity.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) �r���h���̃f�B���N�g���\���𐮗�
 * (2) VSS�T�[�o���_��ύX
 * 
 * 8     06/09/15 16:58 Tsujimura543
 * printReport() �֘A�֐��Q�� simanity.c ���番������
 * 
 * 7     06/09/15 16:52 Tsujimura543
 * getReport() �� simanity.c ���番������
 * 
 * 6     06/09/15 16:17 Tsujimura543
 * deleteMod(), logBeautify(), printLog(), printTracerLog() �� simanity.c
 * ���番������
 * 
 * 5     06/09/15 16:01 Tsujimura543
 * visitSima(), getArea(), getMapURL() �� simanity.c ���番������
 * 
 * 4     06/09/15 15:25 Tsujimura543
 * main(), usage(), setParam() �� simanity.c ���番������
 * 
 * 3     06/09/12 12:20 Tsujimura543
 * ���t�@�N�^�����O���{
 * 
 * 2     06/09/12 11:27 Tsujimura543
 * Visual C++ 6 �ł��r���h�ł��邱�Ƃ��m�F
 * 
 * 1c    06/09/12 7:49 tsupo
 * �����ł̊����󋵕񍐂��\������悤�ɂ��Ă݂�(�~�T�C�����e�󋵁A��͔h��
 * ��)
 * 
 * 1b    06/09/03 20:13 tsupo
 * cookie �܂����C��
 * 
 * 1a    06/08/31 7:26 tsupo
 * �u�~�T�C�����˃C�x���g�v�ɑΉ����邽�߁AdeleteMod() �� logBeautify()
 * ���C��
 * 
 * 1     06/08/30 18:10 tsupo
 * �ŏ��̔� (����m�F�ς�)
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/simanity.c 1     09/05/14 4:45 tsupo $";
#endif

BOOL
loginSimanity( const char *username, const char *password,
               char *cookie, char *simaID, char *hash,
               char *request,
               char *response, size_t sz )
{
    char    url[MAX_URLLENGTH];
    BOOL    ret = FALSE;

    strcpy( url, "http://sima.jp/do_normal.php" );
    sprintf( request,
             "cmd=login&"
             "username=%s&"
             "password=%s",
             username,
             password );
    setUpReceiveBuffer( response, sz );
    http_postEx( url, "application/x-www-form-urlencoded",
                 request, response, cookie ); 
    if ( *response ) {
        // login ����
        const char  *p, *q, *r;

        p = response;
        q = strstr( p, "name=\"c_member_id\" value=\"" );
        if ( q ) {
            q += 26;
            r = strchr( q, '"' );
            if ( r ) {
                strncpy( simaID, q, r - q );
                simaID[r - q] = NUL;
                p = r + 1;
            }
        }

        q = strstr( p, "name=\"user_hash\" value=\"" );
        if ( q ) {
            q += 24;
            r = strchr( q, '"' );
            if ( r ) {
                strncpy( hash, q, r - q );
                hash[r - q] = NUL;
                p = r + 1;
            }
        }

        ret = TRUE;
    }

    return ( ret );
}
