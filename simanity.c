/*
 *  「しまにてぃ」の自分の島の現在の状況を取得、表示する
 *          wriietn by H.Tsujimura  30 Aug 2006
 *
 * History:
 * $Log: /comm/simanity/simanity.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) ビルド環境のディレクトリ構造を整理
 * (2) VSSサーバ拠点を変更
 * 
 * 8     06/09/15 16:58 Tsujimura543
 * printReport() 関連関数群を simanity.c から分離した
 * 
 * 7     06/09/15 16:52 Tsujimura543
 * getReport() を simanity.c から分離した
 * 
 * 6     06/09/15 16:17 Tsujimura543
 * deleteMod(), logBeautify(), printLog(), printTracerLog() を simanity.c
 * から分離した
 * 
 * 5     06/09/15 16:01 Tsujimura543
 * visitSima(), getArea(), getMapURL() を simanity.c から分離した
 * 
 * 4     06/09/15 15:25 Tsujimura543
 * main(), usage(), setParam() を simanity.c から分離した
 * 
 * 3     06/09/12 12:20 Tsujimura543
 * リファクタリング実施
 * 
 * 2     06/09/12 11:27 Tsujimura543
 * Visual C++ 6 でもビルドできることを確認
 * 
 * 1c    06/09/12 7:49 tsupo
 * 他島での活動状況報告も表示するようにしてみた(ミサイル着弾状況、戦艦派遣
 * 状況)
 * 
 * 1b    06/09/03 20:13 tsupo
 * cookie まわりを修正
 * 
 * 1a    06/08/31 7:26 tsupo
 * 「ミサイル発射イベント」に対応するため、deleteMod() と logBeautify()
 * を修正
 * 
 * 1     06/08/30 18:10 tsupo
 * 最初の版 (動作確認済み)
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
        // login 成功
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
