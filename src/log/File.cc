/*
 * DEBUG: section 50    Log file handling
 * AUTHOR: Duane Wessels
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#include "squid.h"
#include "fde.h"
#include "log/File.h"
#include "log/ModDaemon.h"
#include "log/ModStdio.h"
#include "log/ModSyslog.h"
#include "log/ModUdp.h"
#include "log/TcpLogger.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
//#include "squid_log.h"
using namespace std;

sql::Driver *driver;
sql::Connection *conn;
sql::PreparedStatement *pstmt;


CBDATA_TYPE(Logfile);

Logfile *
logfileOpen(const char *path, size_t bufsz, int fatal_flag)
{
    int ret;
    const char *patharg;

    try
    {
//defing the sql connection variables

/*	    string host,usr,pass,db,con;


            ifstream inn;
            inn.open("/root/c++/db_conn.txt");

            if(inn.is_open())
            {

            inn>>host;
            inn>>usr;
            inn>>pass;
            inn>>db;

            inn.close();
            }
            else
            {
    //      h="127.0.0.1:3306";
    //      u="root";
    //	    p="simple";
    //      db="squid";
		    cout<<"\nNot able to open db conf file. Taking default value\n";
            }

            con="tcp://"+host;*/


      double et=66666.660;
/*        int rt=666;
        string cip = "666.66.6.66";
        string rs = "TCP_MISS";
        string hs = "666";
        int rsize=6666;
        string rm = "GET";
        string url = "http://kaw.stb00.s-msn.com/i/7C/C17EADA0AF6BBF29D7C8F9DA4176CC.jpg";
        string user = "mu";
        string hier = "HIER_DIRECT";
        string dest = "6.6.6.6";
        string ct = "image/jpeg";*/

        driver = get_driver_instance();

      conn = driver->connect("tcp://127.0.0.1:3306","root","simple");
//	conn = driver->connect(con,usr,pass);
        conn->setSchema("squid");

//        pstmt = conn->prepareStatement("insert into access_log_mdt(time_since_epoch,response_time,client_src_ip_addr,squid_request_status,http_status_code,reply_size,request_method,request_url,username,squid_hier_status,server_ip_addr,mime_type) values(?,?,?,?,?,?,?,?,?,?,?,?);");

//		pstmt = conn->prepareStatement("insert into access_log_mdt(time_since_epoch,date_day,date_time,response_time,client_src_ip_addr,squid_request_status,http_status_code,reply_size,request_method,request_url,username,squid_hier_status,server_ip_addr,mime_type) values(?,CURDATE(),CURTIME(),?,?,?,?,?,?,?,?,?,?,?);");

		pstmt = conn->prepareStatement("insert into access_log_wt(time_since_epoch,response_time,client_src_ip_addr,squid_request_status,http_status_code,reply_size,request_method,request_url,username,squid_hier_status,server_ip_addr,mime_type) values(?,?,?,?,?,?,?,?,?,?,?,?);");

/*	pstmt->setDouble(1,et);
        pstmt->setInt(2,rt);
        pstmt->setString(3,cip);
        pstmt->setString(4,rs);
        pstmt->setString(5,hs);
        pstmt->setInt(6,rsize);
        pstmt->setString(7,rm);
        pstmt->setString(8,url);
        pstmt->setString(9,user);
        pstmt->setString(10,hier);
        pstmt->setString(11,dest);
        pstmt->setString(12,ct);


        pstmt->executeUpdate();*/

	}

    catch (sql::SQLException &e)
    {
//        cout<<"ERROR"<<e.what();
		fatalf("Connection to Database failed ");
//		exit(1);
    }



    debugs(50, DBG_IMPORTANT, "Logfile: opening log " << path);
    CBDATA_INIT_TYPE(Logfile);

    Logfile *lf = cbdataAlloc(Logfile);
    xstrncpy(lf->path, path, MAXPATHLEN);
    patharg = path;
    /* need to call the per-logfile-type code */
    if (strncmp(path, "stdio:", 6) == 0) {
        patharg = path + 6;
        ret = logfile_mod_stdio_open(lf, patharg, bufsz, fatal_flag);
    } else if (strncmp(path, "daemon:", 7) == 0) {
        patharg = path + 7;
        ret = logfile_mod_daemon_open(lf, patharg, bufsz, fatal_flag);
    } else if (strncmp(path, "tcp:", 4) == 0) {
        patharg = path + 4;
        ret = Log::TcpLogger::Open(lf, patharg, bufsz, fatal_flag);
    } else if (strncmp(path, "udp:", 4) == 0) {
        patharg = path + 4;
        ret = logfile_mod_udp_open(lf, patharg, bufsz, fatal_flag);
#if HAVE_SYSLOG
    } else if (strncmp(path, "syslog:", 7) == 0) {
        patharg = path + 7;
        ret = logfile_mod_syslog_open(lf, patharg, bufsz, fatal_flag);
#endif
    } else {
        debugs(50, DBG_IMPORTANT, "WARNING: log name now starts with a module name. Use 'stdio:" << patharg << "'");
        snprintf(lf->path, MAXPATHLEN, "stdio:%s", patharg);
        ret = logfile_mod_stdio_open(lf, patharg, bufsz, fatal_flag);
    }
    if (!ret) {
        if (fatal_flag)
            fatalf("logfileOpen: %s: couldn't open!\n", path);
        else
            debugs(50, DBG_IMPORTANT, "logfileOpen: " << path << ": couldn't open!");
        lf->f_close(lf);
        cbdataFree(lf);
        return NULL;
    }
    assert(lf->data != NULL);

    if (fatal_flag)
        lf->flags.fatal = 1;

    lf->sequence_number = 0;

    return lf;
}

void
logfileClose(Logfile * lf)
{

    	pstmt->setInt(1,888);

        pstmt->executeUpdate();

        delete pstmt;
        delete conn;
	

    debugs(50, DBG_IMPORTANT, "Logfile: closing log " << lf->path);
    lf->f_flush(lf);
    lf->f_close(lf);
    cbdataFree(lf);
}

void
logfileRotate(Logfile * lf)
{
    debugs(50, DBG_IMPORTANT, "logfileRotate: " << lf->path);
    lf->f_rotate(lf);
}

void
logfileWrite(Logfile * lf, char *buf, size_t len)
{
    lf->f_linewrite(lf, buf, len);
}

void
logfilePrintf(Logfile * lf, const char *fmt,...)
{
    va_list args;
    char buf[8192];
    int s;

    va_start(args, fmt);

    s = vsnprintf(buf, 8192, fmt, args);

    if (s > 8192) {
        s = 8192;

        if (fmt[strlen(fmt) - 1] == '\n')
            buf[8191] = '\n';
    }

    logfileWrite(lf, buf, (size_t) s);
    va_end(args);
}

void
logfileLineStart(Logfile * lf)
{
    lf->f_linestart(lf);
}

void
logfileLineEnd(Logfile * lf)
{
    lf->f_lineend(lf);
    ++ lf->sequence_number;
}

void
logfileFlush(Logfile * lf)
{
    lf->f_flush(lf);
}
