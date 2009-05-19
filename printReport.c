/*
 *  「しまにてぃ」の「自分のしま」の現況を解析、表示する
 *          wriietn by H.Tsujimura  30 Aug 2006 / 15 Sep 2006
 *
 * History:
 * $Log: /comm/simanity/printReport.c $
 * 
 * 1     09/05/14 4:45 tsupo
 * (1) ビルド環境のディレクトリ構造を整理
 * (2) VSSサーバ拠点を変更
 * 
 * 2     07/02/15 19:32 Tsujimura543
 * recordTargets() を修正し、同じ島のログが複数回掲載されてしまう現象を回
 * 避
 * 
 * 1     06/09/15 16:57 Tsujimura543
 * printReport() 関連関数群を simanity.c から分離し、printReport.c を作成
 */

#include "xmlRPC.h"
#include "simanity.h"

#ifndef	lint
static char	*rcs_id =
"$Header: /comm/simanity/printReport.c 1     09/05/14 4:45 tsupo $";
#endif

void
printMissileLog( long targetIslandId, const char *targetIslandName,
                 long turn,
                 long from,
                 const char *islandName,
                 const char *area,
                 const char *simaID,
                 const char *pass,
                 char *cookie )
{
    char    request[BUFSIZ];
    char    *response;
    size_t  sz = MAX_CONTENT_SIZE * 20;
    
    response = (char *)malloc( sz );
    if ( !response )
        return;
    setUpReceiveBuffer( response, sz );
    sprintf( request,
             "current=list&"
             "area=%s&"
             "id=sima%s&"
             "password=%s&"
             "message=frames&"
             "submessage=island&"
             "name=&"
             "islandid=%d&"
             "plan=&"
             "from=%d&"
             "comment=",
             area, simaID, pass, targetIslandId, from );
    http_postEx( "http://g.sima.jp/cgi-bin/n/island.cgi",
                 "application/x-www-form-urlencoded",
                 request, response, cookie );

    if ( *response ) {
        char    *p, *q, *r;
#ifdef  _DEBUG
        FILE    *fp = fopen( "./target.html", "w" );

        if ( fp ) {
            fputs( response, fp );
            fclose( fp );
        }
#endif
        p = strstr( response, "var tvLog = [\"" );
        if ( p ) {
            p += 13;
            q = strstr( p, "];" );
            if ( q ) {
                char    temp[32];

                sprintf( temp, "ターン%d：:p:", turn );
                r = strstr( p, temp );
                if ( r && (r < q) ) {
                    char    *s, *t, *u;
                    char    buf[BUFSIZ];

                    s = strstr( r, "\",\"" );
                    if ( s ) {
                        strncpy( buf, r, s - r );
                        buf[s - r] = NUL;
                        if ( strstr( buf, islandName ) )
                            printTargetLog( buf, targetIslandName );
                    }

                    t = r + 6;

                    while ( (u = strstr( t, temp )) != NULL ) {
                        if ( u >= q )
                            break;

                        s = strstr( u, "\",\"" );
                        if ( s ) {
                            strncpy( buf, u, s - u );
                            buf[s - u] = NUL;
                            if ( strstr( buf, islandName ) )
                               printTargetLog( buf, targetIslandName );
                        }
                        t = u + 6;
                    }
                }
            }
        }
    }

    free( response );
}

const char  *
printCurrentStatus( const char *log, const char *areaName, long turn,
                    long *from,
                    char *islandName )
{
    const char *p, *q, *r;

    p = log;
    q = strstr( p, "var tvOwnIsland = [" );
    if ( !q )
        q = strstr( p, "var tvCurIsland = [" );
    if ( q ) {
        long    l;

        // しまID
        q += 19;
        r = strchr( q, ',' );
        if ( !r )
            return ( p );
        p = r + 1;

        // 順位
        l = atol( p );
        if ( l > 0 )
            *from = l;
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        p = r + 1;

        // 島の名前
        q = strchr( p, '"' );
        if ( !q )
            return ( p );
        q++;
        r = strchr( q, '"' );
        if ( !r )
            return ( p );
        strncpy( islandName, q, r - q );
        islandName[r - q] = NUL;
        p = r + 1;
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        p = r + 1;

        if ( turn > 0 )
            printf( "== ターン%d の状況報告 ==\n", turn );
        if ( areaName[0] )
            printf( "%s ", areaName );
        printf( "%s島: 現在、%d位", islandName, l );

        // ポイント
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "  %dpt", l );
        p = r + 1;

        // 人口(x100人)
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "\n  人口: %d00人", l );
        p = r + 1;

        // 面積(x100万坪)
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "  面積: %d00万坪", l );
        p = r + 1;

        // 資金(億円)
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "\n  資金: %d億円", l );
        p = r + 1;

        // 食料
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "  食料: %dトン", l );
        p = r + 1;

        // 資源
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "  資源: %dトン", l );
        p = r + 1;

        // 農場(x千人)
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "\n  農場: %d000人規模", l );
        p = r + 1;

        // 工場(x千人)
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "  工場: %d000人規模", l );
        p = r + 1;

        // 資源生産(x千人)
        l = atol( p );
        r = strchr( p, ',' );
        if ( !r )
            return ( p );
        printf( "  資源生産: %d000人規模", l );
        p = r + 1;

        fputs( "\n", stdout );
    }

    return ( p );
}


typedef struct targets  {
    long    targetIslandID;
    char    targetIslandName[80];
}   TARGET;

#define initTargets(n)  recordTargets(n)

TARGET  *
recordTargets( int *numOfTargets )
{
    const  int      unit = 20;
    static int      cnt  = 1;
    static TARGET   *targets = NULL;

    if ( !targets ) {
        *numOfTargets = 0;
        targets = (TARGET *)malloc( sizeof ( TARGET ) * unit * cnt );
        if ( !targets ) {
            fputs( "memory exhausted.\n", stderr );
            return ( NULL );
        }
    }
    else {
        BOOL    flag = FALSE;

        if ( *numOfTargets == 0 )
            flag = TRUE;
        else {
            int     i;
            BOOL    found = FALSE;

            for ( i = 0; i < *numOfTargets; i++ ) {
                if ( targets[i].targetIslandID ==
                                targets[*numOfTargets].targetIslandID ) {
                    found = TRUE;
                    break;
                }
            }
            if ( found == FALSE )
                flag = TRUE;
        }

        if ( flag == TRUE ) {
            (*numOfTargets)++;
            if ( *numOfTargets >= unit * cnt ) {
                TARGET  *p;

                cnt++;
                p = (TARGET *)
                        realloc( targets, sizeof ( TARGET ) * unit * cnt );
                if ( p )
                    targets = p;
                else
                    (*numOfTargets)--;
            }
        }
    }
    targets[*numOfTargets].targetIslandID      = 0;
    targets[*numOfTargets].targetIslandName[0] = NUL;

    return ( targets );
}

void
printHistory( const char *log, long turn, long from,
              const char *islandName,
              const char *area,
              const char *simaID,
              const char *pass,
              char       *cookie )
{
    const char  *p, *q, *r;

    p = log;
    q = strstr( p, "var tvLog = [\"" );
    if ( q ) {
        const char  *s;
        const char  *e;
        const char  *t, *u;
        char        temp[BUFSIZ];
        long        numOfTargets = 0;
        TARGET      *targets = NULL;
        int         i;

        targets = initTargets( &numOfTargets );
        if ( !targets )
            return;

        q += 13;
        s = q;
        e = strstr( q, "var " );
        if ( e && (turn > 0) ) {
            printf( "\n最近の出来事\n" );
            while ( turn > 0 ) {
                targets[numOfTargets].targetIslandID      = 0;
                targets[numOfTargets].targetIslandName[0] = NUL;
                sprintf( temp, "ターン%d：:p:", turn );
                r = strstr( q, temp );
                if ( r && (r < e) ) {
                    printLog( r,
                              &(targets[numOfTargets].targetIslandID),
                              targets[numOfTargets].targetIslandName );
                    if ( targets[numOfTargets].targetIslandID > 0 )
                        targets = recordTargets( &numOfTargets );
                    t = r + 6;

                    while ( (u = strstr( t, temp )) != NULL ) {
                        if ( u >= e )
                            break;

                        printLog( u,
                                  &(targets[numOfTargets].targetIslandID),
                                  targets[numOfTargets].targetIslandName );
                        if ( targets[numOfTargets].targetIslandID > 0 )
                            targets = recordTargets( &numOfTargets );
                        t = u + 6;
                    }
                }
                else
                    break;
                e = r;

                /* 他島に発射したミサイルの着弾結果を表示 */
                for ( i = 0; i < numOfTargets; i++ ) {
                    if ( (targets[i].targetIslandID > 0) &&
                         targets[i].targetIslandName[0]  &&
                         strcmp(targets[i].targetIslandName, islandName) )
                        printMissileLog( targets[i].targetIslandID,
                                         targets[i].targetIslandName,
                                         turn, from,
                                         islandName, area, simaID, pass,
                                         cookie );
                }

                turn--;
                numOfTargets = 0;
            }
        }

        if ( targets )
            free( targets );
    }
}

void
printReport( const char *area,
             const char *simaID,
             const char *pass,
             const char *response,
             char       *cookie )
{
    long        turn = 0;
    long        from = 0;
    char        areaName[16];
    char        islandName[80];
    const char  *p, *q, *r;

    areaName[0]   = NUL;
    islandName[0] = NUL;
    p = response;
#ifdef  _DEBUG
    {
        FILE    *fp = fopen( "./sima.html", "w" );
        if ( fp ) {
            fputs( response, fp );
            fclose( fp );
        }
    }
#endif

    // 現在のターン
    q = strstr( p, "var tvTurn = " );
    if ( q )
        turn = atol( q + 13 );

    // 海域名
    q = strstr( p, "var tvAreaname = \"" );
    if ( q ) {
        q += 18;
        r = strchr( q, '"' );
        if ( r ) {
            strncpy( areaName, q, r - q );
            areaName[r - q] = NUL;
        }
    }

    if ( turn > 0 ) {
        // 自分の島の現況
        if ( areaName[0] )
            p = printCurrentStatus( p, areaName, turn,
                                    &from,
                                    islandName );

        // 自分の島の過去ログ
        if ( p )
            printHistory( p, turn, from,
                          islandName,
                          area, simaID, pass, cookie );

        // 記帳の有無
        if ( !p )
            p = response;
        q = strstr( p, "var tvBbsTurn = " );
        if ( q ) {
            q += 16;
            turn = atol( q );
            if ( turn > 0 )
                printf( "\nターン%dに記帳がありました\n", turn );
        }
    }
}
