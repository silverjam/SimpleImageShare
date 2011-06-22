#include <QtCore/QString>
#include <QtTest/QtTest>

#include "sislibrary.h"
#include "sisserverlibrary.h"
#include "sisclient.h"

#include <algorithm>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class SISLibraryTest : public QObject
{
    Q_OBJECT

public:
    SISLibraryTest();

private Q_SLOTS:
    void testProtoVersion();
    void testDiscoveredImageSets();
    void testServer();
    void testDataPool();
    void testDataPool2();
    void testClientList();
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SISLibraryTest::SISLibraryTest()
{
}

// **************************************************************************
class TestCommanSink : public CommandSink
{
    Q_OBJECT

public:
    TestCommanSink()
        : m_b_handle_DiscoveredImageSets(false)
        , m_b_handle_ProtocolVersion(false)
    {
        connect(this, SIGNAL(incoming_ProtocolVersion(int)), this, SLOT(handle_ProtocolVersion(int)));
        connect(this, SIGNAL(incoming_DiscoverImageSets(int)), this, SLOT(handle_DiscoverImageSets(int)));
    }

    quint32 m_uDiscoveredImageSets;
    bool m_b_handle_DiscoveredImageSets;
    quint32 m_uProtocolVersion;
    bool m_b_handle_ProtocolVersion;

public slots:
    void handle_DiscoverImageSets(int input)
    {
        m_b_handle_DiscoveredImageSets = true;
        m_uDiscoveredImageSets = input;
    }

    void handle_ProtocolVersion(int input)
    {
        m_b_handle_ProtocolVersion = true;
        m_uProtocolVersion = input;
    }

public:
    bool nothingCalled()
    {
        return
            ! m_b_handle_DiscoveredImageSets &&
            ! m_b_handle_ProtocolVersion
        ;
    }
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testProtoVersion()
{
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);

    TestCommanSink* pSink = new TestCommanSink;

    QDataStream ds1(&buf);
    SisCommandBuilder::build_ProtocolVersion(ds1);

    buf.seek(0);
    QDataStream ds2(&buf);

    SisCommandParser cmds2(pSink);
    QVERIFY( cmds2.headerSize() <= buf.size() );

    quint32 commandId = cmds2.readHeader(ds2);
    qint64 commandSize = cmds2.discoverSize(commandId);

    QVERIFY( commandSize <= (buf.size() - buf.pos()) );
    QVERIFY( cmds2.parseOne(ds2, commandId) );

    QVERIFY( pSink->m_b_handle_ProtocolVersion );
    QVERIFY( ! pSink->nothingCalled() );

    QVERIFY( pSink->m_uProtocolVersion == CURRENT_PROTOCOL_VERSION );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testDiscoveredImageSets()
{
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);

    TestCommanSink* pSink = new TestCommanSink;

    QDataStream ds1(&buf);

    SisCommandBuilder::build_DiscoveredImageSets(ds1, 42);

    buf.seek(0);
    QDataStream ds2(&buf);

    SisCommandParser cmds2(pSink);
    QVERIFY( cmds2.headerSize() <= buf.size() );

    quint32 commandId = cmds2.readHeader(ds2);
    qint64 commandSize = cmds2.discoverSize(commandId);

    QVERIFY( commandSize <= (buf.size() - buf.pos()) );
    QVERIFY( cmds2.parseOne(ds2, commandId) );

    QVERIFY( pSink->m_b_handle_DiscoveredImageSets );
    QVERIFY( ! pSink->nothingCalled() );

    QVERIFY( pSink->m_uDiscoveredImageSets == 42 );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct TestServerSink : public CommandSink
{
    Q_OBJECT

public slots:

    void handle_ProtocolVersion(int version) {
        bCalledProto = true;
        QVERIFY( version == CURRENT_PROTOCOL_VERSION );
    }

public:
    TestServerSink() {
        bCalledProto = false;
        connect(this, SIGNAL(incoming_ProtocolVersion(int)), this, SLOT(handle_ProtocolVersion(int)));
    }

    bool bCalledProto;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testServer()
{
    int argc = 1;
    const char* argv[] = { "test" };

    QCoreApplication app(argc, (char**)argv);
    (void)app;

    TestServerSink* pSinkServer(new TestServerSink);
    SisServer server(pSinkServer, QHostAddress::LocalHost);

    TestServerSink* pSinkClient(new TestServerSink);
    SisClient client("127.0.0.1", server.port(), pSinkClient);

    client.connectToHost();

    QEventLoop loop;
    QObject::connect(&client, SIGNAL(connected()), &loop, SLOT(quit()));

    loop.exec();

    client.send_ProtocolVersion(0);

    QVERIFY( QObject::connect(&server, SIGNAL(dataProcessed()), &loop, SLOT(quit())) );
    loop.exec();

    QVERIFY( QObject::disconnect(&server, SIGNAL(dataProcessed()), &loop, SLOT(quit())) );

    QVERIFY( pSinkServer->bCalledProto );

#if 0 // TODO
    QTcpSocket& clientSocket =  client.socket();
    server.send_ProtocolVersion(&clientSocket);

    QVERIFY( QObject::connect(&client, SIGNAL(dataProcessed()), &loop, SLOT(quit())) );
    loop.exec();

    QVERIFY( pSinkClient->bCalledProto );
#endif
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testDataPool()
{
    DataPool pool;
    pool.setChunkSize(4);

    QBuffer buffer;
    QVERIFY( buffer.open(QIODevice::ReadWrite) );

    QDataStream ds(&buffer);

    SisCommandBuilder::build_DiscoveredImageSets(ds, 102);

    pool.poolNewData(buffer.data().data(), 2);
    QVERIFY( ! pool.isDataReady() );

    pool.poolNewData(buffer.data().data() + 2, 2);

    QVERIFY( pool.isDataReady() );

    QByteArray bytes;
    QVERIFY( pool.readPooledChunk(bytes) );

    QVERIFY( bytes.size() == 4 );
    QVERIFY( memcmp(bytes.data(), buffer.data(), bytes.size()) == 0 );

    pool.poolNewData(buffer.data().data() + 4, 2);

    QVERIFY( ! pool.readPooledChunk(bytes) );

    pool.poolNewData(buffer.data().data() + 6, 2);

    QVERIFY( pool.readPooledChunk(bytes) );
    QVERIFY( bytes.size() == 4 );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testDataPool2()
{
    char buf[] = { 1, 2, 3, 4};

    DataPool pool;
    pool.setChunkSize(sizeof(buf));

    pool.poolNewData(buf, sizeof(buf));

    QByteArray readBuffer;

    QVERIFY( pool.readPooledChunk(readBuffer) );

    QVERIFY( readBuffer.size() == sizeof(buf) );
    QVERIFY( ::memcmp(readBuffer.data(), buf, sizeof(buf)) == 0 );

    char buf2[] = { 5, 6, 7, 8, 9 };
    pool.setChunkSize(sizeof(buf2));

    pool.poolNewData(buf2, sizeof(buf2));

    QByteArray readBuffer2;

    QVERIFY( pool.readPooledChunk(readBuffer2) );
    QVERIFY( readBuffer2.size() == sizeof(buf2) );

    QVERIFY( ::memcmp(readBuffer2.data(), buf2, sizeof(buf2)) == 0 );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testClientList()
{
    int argc = 1;
    const char* argv[] = { "test" };

    QCoreApplication app(argc, (char**)argv);
    (void)app;

    TestServerSink* pSinkServer(new TestServerSink);
    SisServer server(pSinkServer, QHostAddress::LocalHost, 0);

    TestServerSink* pSinkClient(new TestServerSink);
    SisClient client("127.0.0.1", server.port(), pSinkClient);

    QVERIFY( server.clients().count() == 0 );

    client.connectToHost();

    QEventLoop loop;
    QObject::connect(&server, SIGNAL(newClient()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY( server.clients().count() == 1 );
    QVERIFY( server.clients().first()->peerPort() == client.socket().localPort() );

    SisClient client2("127.0.0.1", server.port(), pSinkClient);
    client2.connectToHost();

    QObject::connect(&server, SIGNAL(newClient()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY( server.clients().count() == 2 );

    client.closeConnection();

    QObject::connect(&server, SIGNAL(lostClient()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY( server.clients().count() == 1 );
    QVERIFY( server.clients().first()->peerPort() == client2.socket().localPort() );

    client2.closeConnection();

    QObject::connect(&server, SIGNAL(lostClient()), &loop, SLOT(quit()));
    loop.exec();

    QVERIFY( server.clients().count() == 0 );
}

QTEST_APPLESS_MAIN(SISLibraryTest)

#include "tst_sislibrarytest.moc"
