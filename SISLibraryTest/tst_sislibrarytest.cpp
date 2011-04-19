#include <QtCore/QString>
#include <QtTest/QtTest>

#include "sislibrary.h"

using namespace CommandInformation;

class SISLibraryTest : public QObject
{
    Q_OBJECT

public:
    SISLibraryTest();

private Q_SLOTS:
    void testProtoVersion();
    void testDiscoveredImageSets();
};

SISLibraryTest::SISLibraryTest()
{
}

class TestCommanSink : public ICommandSink
{
public:
    TestCommanSink()
        : m_b_handle_DiscoveredImageSets(false)
        , m_b_handle_ProtocolVersion(false)
    {}

    DiscoveredImageSets m_DiscoveredImageSets;
    bool m_b_handle_DiscoveredImageSets;
    void handle_DiscoveredImageSets(const DiscoveredImageSets& input)
    {
        m_b_handle_DiscoveredImageSets = true;
        m_DiscoveredImageSets = input;
    }

    ProtocolVersion m_ProtocolVersion;
    bool m_b_handle_ProtocolVersion;
    void handle_ProtocolVersion(const ProtocolVersion& input)
    {
        m_b_handle_ProtocolVersion = true;
        m_ProtocolVersion = input;
    }

    bool nothingCalled()
    {
        return
            ! m_b_handle_DiscoveredImageSets &&
            ! m_b_handle_ProtocolVersion
        ;
    }
};

void SISLibraryTest::testProtoVersion()
{
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);

    TestCommanSink sink;

    QDataStream ds1(&buf);

    SisCommands cmds1(ds1, &sink);
    cmds1.build_ProtocolVersion();

    QVERIFY( sink.nothingCalled() );

    buf.seek(0);
    QDataStream ds2(&buf);

    SisCommands cmds2(ds2, &sink);
    QVERIFY( cmds2.parseOne() );

    QVERIFY( sink.m_b_handle_ProtocolVersion );
    QVERIFY( ! sink.nothingCalled() );
}

void SISLibraryTest::testDiscoveredImageSets()
{
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);

    TestCommanSink sink;

    QDataStream ds1(&buf);

    SisCommands cmds1(ds1, &sink);
    cmds1.build_DiscoveredImageSets(42);

    QVERIFY( sink.nothingCalled() );

    buf.seek(0);
    QDataStream ds2(&buf);

    SisCommands cmds2(ds2, &sink);
    QVERIFY( cmds2.parseOne() );

    QVERIFY( sink.m_b_handle_DiscoveredImageSets );
    QVERIFY( ! sink.nothingCalled() );

    QVERIFY( sink.m_DiscoveredImageSets.count == 42 );
}

QTEST_APPLESS_MAIN(SISLibraryTest);

#include "tst_sislibrarytest.moc"
