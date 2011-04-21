#include <QtCore/QString>
#include <QtTest/QtTest>

#include "sislibrary.h"
#include "sisserverlibrary.h"

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
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SISLibraryTest::SISLibraryTest()
{
}

// **************************************************************************
class TestCommanSink : public ICommandSink
{
public:
    TestCommanSink()
        : m_b_handle_DiscoveredImageSets(false)
        , m_b_handle_ProtocolVersion(false)
    {}

    quint32 m_uDiscoveredImageSets;
    bool m_b_handle_DiscoveredImageSets;
    void handle_DiscoveredImageSets(int input)
    {
        m_b_handle_DiscoveredImageSets = true;
        m_uDiscoveredImageSets = input;
    }

    quint32 m_uProtocolVersion;
    bool m_b_handle_ProtocolVersion;
    void handle_ProtocolVersion(int input)
    {
        m_b_handle_ProtocolVersion = true;
        m_uProtocolVersion = input;
    }

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

    TestCommanSink sink;

    QDataStream ds1(&buf);
    SisCommandBuilder::build_ProtocolVersion(ds1);

    buf.seek(0);
    QDataStream ds2(&buf);

    SisCommandParser cmds2(&sink);
    QVERIFY( cmds2.headerSize() <= buf.size() );

    quint32 commandId = cmds2.readHeader(ds2);
    qint64 commandSize = cmds2.discoverSize(commandId);

    QVERIFY( commandSize <= (buf.size() - buf.pos()) );
    QVERIFY( cmds2.parseOne(ds2, commandId) );

    QVERIFY( sink.m_b_handle_ProtocolVersion );
    QVERIFY( ! sink.nothingCalled() );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testDiscoveredImageSets()
{
//    QBuffer buf;
//    buf.open(QIODevice::ReadWrite);

//    TestCommanSink sink;

//    QDataStream ds1(&buf);

//    SisCommands cmds1(ds1, &sink);
//    cmds1.build_DiscoveredImageSets(42);

//    QVERIFY( sink.nothingCalled() );

//    buf.seek(0);
//    QDataStream ds2(&buf);

//    SisCommands cmds2(ds2, &sink);
//    QVERIFY( cmds2.parseOne() );

//    QVERIFY( sink.m_b_handle_DiscoveredImageSets );
//    QVERIFY( ! sink.nothingCalled() );

//    QVERIFY( sink.m_DiscoveredImageSets.count == 42 );
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
SISLibraryTest::testServer()
{
    //Server server(QHostAddress::LocalHost);

}

QTEST_APPLESS_MAIN(SISLibraryTest);

#include "tst_sislibrarytest.moc"
