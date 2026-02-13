#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct nchainTest: public byeolTest {};

namespace {
    class myNode: public node {
        BY(CLASS(myNode, node))

    public:
        myNode(int num): number(num) {}

        scope& subs() override { return dumScope::singleton(); }

        priorType prioritize(const args& types) const override { return NO_MATCH; }

        str eval(const args& a) override { return str(); }

        int number;
    };

    void simpleAddDelTest(int cnt) {
        tnmap<std::string, myNode>* m = new tnmap<std::string, myNode>();
        tnchain<std::string, myNode> chn(*m);
        ASSERT_EQ(0, m->len());
        ASSERT_EQ(chn.len(), m->len());
        ASSERT_FALSE(chn.getNext());

        std::map<std::string, myNode*> tray;
        for(int n = 0; n < cnt; n++) {
            myNode* new1 = new myNode(n);
            m->add("name" + std::to_string(n), new1);
            tray.insert({"name" + std::to_string(n), new1});
        }

        ASSERT_EQ(chn.len(), cnt);
        ASSERT_EQ(chn.len(), cnt);
        ASSERT_EQ(chn.len(), cnt);
        ASSERT_EQ(chn.len(), cnt);

        ASSERT_FALSE(chn.getNext());
        for(auto e = chn.begin(); e; ++e) {
            ASSERT_TRUE(e.getKey());
            ASSERT_NE(*e.getKey(), "");
            const myNode* elem = e.getVal();
            const myNode* answer = tray[*e.getKey()];
            ASSERT_EQ(elem, answer);
            ASSERT_EQ("name" + std::to_string(elem->number), *e.getKey());
        }
    }

    class myMyNode: public myNode {
        BY(CLASS(myMyNode, myNode))

    public:
        myMyNode(int num): super(num) {}
    };

    void examineChain2Element(nchain& chn, int val1, int val2) {
        ASSERT_TRUE(chn.in(std::to_string(val1)));
        ASSERT_TRUE(chn.in(std::to_string(val2)));
    }

    struct myNode2: public myNode {
        BY(CLASS(myNode2, myNode))

    public:
        myNode2(int val): super(val) {}
    };

    bool vectorHas(std::vector<float>& v, float val) {
        for(int n = 0; n < v.size(); n++)
            if(v[n] == val) return true;
        return false;
    }

    template <typename T = myNode>
    static nbool isMyNodesHasEqualIntArray(const tnchain<float, myNode>& root, float expects[], int expectSize) {
        vector<float> actuals;
        for(const auto& elem: root)
            if(elem.isSub<T>()) actuals.push_back((float) elem.number);

        if(actuals.size() != expectSize) return false;
        for(int n = 0; n < expectSize; n++)
            if(!vectorHas(actuals, expects[n])) return false;
        return true;
    }

} // namespace

TEST_F(nchainTest, simpleAddDelTest10) { simpleAddDelTest(10); }

TEST_F(nchainTest, simpleAddDelTest1000) { simpleAddDelTest(1000); }

TEST_F(nchainTest, simpleAddDelTest10000) { simpleAddDelTest(10000); }

TEST_F(nchainTest, testucontainableAPI) {
    //  initial state:
    tstr<nchain> arr(new nchain());
    bicontainable* con = arr.get();
    ASSERT_EQ(con->len(), 0);

    auto head = arr->begin();
    ASSERT_TRUE(head.isEnd());
    auto tail = con->end();
    ASSERT_TRUE(tail.isEnd());

    ASSERT_TRUE(con->add("0", new myNode(0)));
    ASSERT_TRUE(con->add("1", new myMyNode(1)));
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);
    ASSERT_EQ(con->len(), 2);

    //  add:
    for(int n = 0; n < con->len(); n++)
        ASSERT_EQ(con->get(std::to_string(n))->cast<myNode>()->number, n);

    //  get & each:
    {
        tnarr<myNode> tray = arr->getAll<myNode>([](const std::string& name, const myNode& elem) { return true; });
        ASSERT_EQ(tray.len(), 2);

        int cnt = 0;
        tray = arr->getAll<myNode>([&cnt](const std::string& name, const myNode& elem) {
            if(cnt >= 1) return false;
            cnt++;
            return true;
        });
        ASSERT_EQ(tray.len(), 1);
    }

    tnarr<myMyNode> tray = arr->getAll<myMyNode>([](const std::string& name, const myMyNode& elem) {
        if(elem.number == 1) return true;
        return false;
    });
    ASSERT_EQ(tray.len(), 1);

    //  del:
    ASSERT_TRUE(con->del("1"));
    ASSERT_EQ(con->len(), 1);
    ASSERT_EQ(con->begin()->cast<myNode>()->number, 0);

    //  add with element:
    narr arr2;
    ASSERT_EQ(arr2.add(*con), 1);
    ASSERT_TRUE(arr2.add(new myNode(1)));
    ASSERT_TRUE(arr2.add(new myMyNode(2)));
    ASSERT_TRUE(arr2.add(new myNode(3)));
    ASSERT_EQ(arr2[2].cast<myNode>()->number, 2);
    ASSERT_EQ(arr2[3].cast<myNode>()->number, 3);
    ASSERT_EQ(arr2.len(), 4);

    auto e = arr2.begin();
    e = e + 2;
    ASSERT_EQ(e->cast<myNode>()->number, 2);
    ASSERT_TRUE(arr2.add(e, new myNode(5)));
    ASSERT_TRUE(arr2.add(2, new myNode(6)));

    ASSERT_EQ(arr2[0].cast<myNode>()->number, 0);
    ASSERT_EQ(arr2[1].cast<myNode>()->number, 1);
    ASSERT_EQ(arr2[2].cast<myNode>()->number, 6);
    ASSERT_EQ(arr2[3].cast<myNode>()->number, 5);
    ASSERT_EQ(arr2[4].cast<myNode>()->number, 2);
    ASSERT_EQ(arr2[5].cast<myNode>()->number, 3);

    ASSERT_EQ(con->len(), 1);
    ncnt count = 0;
    for(auto e = arr2.iterate(1); e != arr2.iterate(3); ++e)
        count += con->add(to_string(count), *e);
    ASSERT_EQ(count, 2);
    ASSERT_EQ(con->len(), 3);

    // con = {0, 1, 6}

    auto e2 = con->begin();
    myNode* elem = e2->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 0);

    elem = (++e2)->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 1);

    elem = (++e2)->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 6);
    ASSERT_FALSE(++e2);

    ASSERT_TRUE(con->len() > 0);
    con->rel();
    ASSERT_TRUE(con->len() == 0);

    count = 0;
    for(auto e = arr2.iterate(2); e; ++e)
        count += con->add(to_string(count), *e);
    ASSERT_EQ(count, 4);
    e2 = con->begin();
    elem = e2->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 6);

    elem = (++e2)->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 5);

    elem = (++e2)->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 2);

    elem = (++e2)->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 3);

    ASSERT_TRUE(con->del(con->begin() + 1, con->begin() + 3));
    ASSERT_EQ(con->len(), 2);
    e2 = con->begin();
    elem = e2->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 6);

    elem = (++e2)->cast<myNode>();
    ASSERT_TRUE(elem);
    ASSERT_EQ(elem->number, 3);
}

TEST_F(nchainTest, testLinkedChainWithOnly1Element) {
    nmap map1;
    map1.add("0", new myNode(0));
    nchain chn1(map1);
    ASSERT_EQ(chn1.len(), 1);

    nmap map2;
    map2.add("1", new myNode(0));
    nchain chn2(map2);
    ASSERT_EQ(chn2.len(), 1);
    chn2.link(chn1);
    ASSERT_EQ(chn2.len(), 2);

    int n = 0;
    for(auto e = chn2.begin(); e; ++e)
        n++;
    ASSERT_EQ(n, 2);
}

TEST_F(nchainTest, testLinkedChainWithNContainerAPI) {
    nmap map1;
    nchain chn1(map1);
    nmap map2;
    nchain chn2(map2);
    vector<int> expectElementNums;

    // add each chains:
    ASSERT_TRUE(chn2.add("6", new myNode(6)));
    ASSERT_TRUE(chn2.add("5", new myMyNode(5)));
    examineChain2Element(chn2, 6, 5);

    nmap arr3;
    nchain chn3(arr3);
    ASSERT_TRUE(chn3.add("2", new myNode(2)));
    ASSERT_TRUE(chn3.add("3", new myMyNode(3)));
    examineChain2Element(chn3, 2, 3);

    ASSERT_TRUE(map1.add("0", new myNode(0)));
    ASSERT_TRUE(chn1.add("1", new myMyNode(1)));
    examineChain2Element(chn1, 0, 1);

    expectElementNums.push_back(0);
    expectElementNums.push_back(1);
    expectElementNums.push_back(6);
    expectElementNums.push_back(5);
    expectElementNums.push_back(2);
    expectElementNums.push_back(3);

    // link:
    ASSERT_FALSE(chn1.getNext());
    ASSERT_FALSE(chn2.getNext());
    ASSERT_FALSE(chn3.getNext());
    ASSERT_TRUE(chn1.link(chn2));
    ASSERT_FALSE(chn1.getNext()->getNext());
    ASSERT_TRUE(chn2.link(chn3));
    ASSERT_FALSE(chn3.getNext());
    ASSERT_FALSE(chn1.getNext() TO(getNext()) TO(getNext()));
    ASSERT_EQ(chn1.len(), 6);

    // add with link:
    ASSERT_EQ(chn1.get<myNode>("0")->number, 0);
    ASSERT_EQ(chn1.get<myNode>("1")->number, 1);
    ASSERT_EQ(chn1.get<myNode>("6")->number, 6);
    ASSERT_EQ(chn1.get<myNode>("5")->number, 5);
    ASSERT_EQ(chn1.get<myNode>("2")->number, 2);
    ASSERT_EQ(chn1.get<myNode>("3")->number, 3);

    // each with link:
    int cnt = 0;
    auto lambda = [&cnt, &expectElementNums](const nbicontainer& chn) -> void {
        for(const auto& e: chn) {
            if(!e.cast<myNode>()) {
                cnt = -1;
                return;
            }
        }

        for(int n = 0; n < chn.len(); ++n, cnt++)
            if(!chn.in(std::to_string(expectElementNums[cnt]))) {
                cnt = -1;
                return;
            }
    };
    lambda(chn1);
    ASSERT_EQ(cnt, 6);

    cnt = 2;
    lambda(chn2);
    ASSERT_EQ(cnt, 6);

    cnt = 4;
    expectElementNums[5] = -1;
    lambda(chn3);
    ASSERT_EQ(cnt, -1);

    // del with link:
    ASSERT_EQ(chn1.len(), 6);
    ASSERT_EQ(chn1.getContainer().len(), 2);
    ASSERT_EQ(chn2.len(), 4);
    ASSERT_EQ(chn2.getContainer().len(), 2);
    ASSERT_EQ(chn3.len(), 2);
    ASSERT_EQ(chn3.getContainer().len(), 2);

    ASSERT_TRUE(chn1.del(chn1.begin() + 1, chn2.begin() + 1));
    ASSERT_EQ(chn1.len(), 4);
    ASSERT_EQ(chn2.len(), 3);
}

TEST_F(nchainTest, testIfnchainLinkItself) {
    nchain chn;
    chn.add("0", new myNode(0));
    chn.add("1", new myNode(1));
    ASSERT_FALSE(chn.link(chn));
    ASSERT_EQ(chn.len(), 2);
}

TEST_F(nchainTest, testcloneDeep) {
    nchain chn;
    chn.add("0", new myNode(0));
    chn.add("1", new myNode(1));
    nchain chn2;
    chn2.add("2", new myNode(2));
    chn2.add("3", new myNode(3));
    chn.link(chn2);

    ASSERT_EQ(chn["0"].cast<myNode>()->number, 0);
    ASSERT_EQ(chn["1"].cast<myNode>()->number, 1);
    ASSERT_EQ(chn["2"].cast<myNode>()->number, 2);
    ASSERT_EQ(chn["3"].cast<myNode>()->number, 3);

    tstr<nchain> it((nchain*) chn.cloneDeep()); // cloneDeep() clones all element in all chained container.
    nchain& itsChn = *it;
    ASSERT_EQ(itsChn["0"].cast<myNode>()->number, 0);
    ASSERT_EQ(itsChn["1"].cast<myNode>()->number, 1);
    ASSERT_EQ(itsChn["2"].cast<myNode>()->number, 2);
    ASSERT_EQ(itsChn["3"].cast<myNode>()->number, 3);

    ASSERT_NE(&itsChn["0"], &chn["0"]);
    ASSERT_NE(&itsChn["1"], &chn["1"]);
    ASSERT_NE(&itsChn["2"], &chn["2"]);
    ASSERT_NE(&itsChn["3"], &chn["3"]);
}

TEST_F(nchainTest, testShouldLinkOverwritePrevious) {
    tstr<nmap> map1Str(new nmap());
    const life& map1tag = life::getBindTag(map1Str.getItsId()) OR_ASSERT(map1tag);
    ASSERT_EQ(map1tag.getStrongCnt(), 1);

    tweak<nmap> map1Weak = *map1Str;
    map1Str->add("0", new myNode(0));
    map1Str->add("1", new myNode(1));
    ASSERT_EQ(map1Str->len(), 2);
    ASSERT_EQ(map1tag.getStrongCnt(), 1);


    nchain chn2;
    chn2.add("2", new myNode(2));
    chn2.add("3", new myNode(3));
    ASSERT_EQ(chn2.len(), 2);

    ASSERT_TRUE(chn2.link(chn2.wrap(*map1Str)));
    ASSERT_EQ(map1tag.getStrongCnt(), 2);
    // chn2 --> unknown chain instance holding map1
    ASSERT_EQ(chn2.len(), 4);

    map1Str.rel();
    ASSERT_EQ(map1tag.getStrongCnt(), 1);
    ASSERT_EQ(chn2.len(), 4);
    ASSERT_TRUE(map1Weak.isBind());

    nmap map2;
    ASSERT_TRUE(map1Weak.isBind());
    chn2.link(chn2.wrap(map2));
    ASSERT_EQ(map1tag.getStrongCnt(), 0);
    // this overwrites chain containing map1. it's now dangling.
    // chn2(2, 3) --> unknown chain instance holding map2(null)
    //   |
    //   +--- X --> unknown chain instance holding map1(0, 1)
    ASSERT_FALSE(map1Weak.isBind());

    ASSERT_EQ(chn2.len(), 2);
}

TEST_F(nchainTest, testDelWithLink) {
    nchain chn;
    chn.add("1", new myNode(1));
    ASSERT_EQ(chn.len(), 1);

    {
        nmap map1;
        map1.add("2", new myNode(2));
        map1.add("3", new myNode(3));

        tstr<nchain> map1Str = chn.wrap(map1);
        chn.link(*map1Str);
        // chn --> map1Str with map1
        //  ^
        //  |
        // head
        ASSERT_EQ(chn.len(), 3);
        tweak<nchain> map2Weak;
        {
            nmap map2;
            map2.add("4", new myNode(4));
            map2.add("5", new myNode(5));
            map2.add("6", new myNode(6));
            tstr<nchain> map2Str = nchain::wrap(map2);
            map1Str->link(*map2Str);
            map2Weak = map2Str;
            // now, chn --> map1Str with map1 --> map2Str with map2
            ASSERT_EQ(map2Str->len(), 3);
            ASSERT_EQ(map1Str->len(), 3 + map1.len());
            ASSERT_EQ(chn.len(), 1 + map1.len() + map2.len());
        }

        map1Str->unlink();
        // now, chn --> map1Str with map1
        ASSERT_FALSE(map2Weak.isBind());
        ASSERT_EQ(chn.len(), 1 + map1.len());

        BY_DI("chn.len()=%d", chn.len());
        auto e = chn.iterate(chn.len() - 1);

        myNode& last = *e->cast<myNode>();
        ASSERT_EQ(last.number, 3);
    }
    chn.unlink();
    ASSERT_EQ(chn.len(), 1);
}

TEST_F(nchainTest, testLastIterator) {
    nchain chn;
    ASSERT_EQ(chn.len(), 0);
    chn.add("1", new myNode(1));
    chn.add("3", new myNode(3));
    chn.add("4", new myNode(2));

    nbool sorted = true;
    ASSERT_TRUE(chn.in("1"));
    ASSERT_TRUE(chn.in("3"));
    ASSERT_TRUE(chn.in("4"));
    ASSERT_TRUE(sorted);
}

TEST_F(nchainTest, testRangeBasedForLoop) {
    nchain map1;
    map1.add("3", new myNode(3));
    map1.add("7", new myNode(7));
    nchain map2;
    map2.add("1", new myNode(1));
    map2.add("2", new myNode(2));
    map1.link(map2);

    int sum = 0;
    for(auto& e: map1) {
        myNode& cast = *e.cast<myNode>();
        sum += cast.number;
    }

    int sum2 = 0;
    for(const node& e: map1) {
        const myNode& cast = *e.cast<myNode>();
        sum2 += cast.number;
    }
    ASSERT_EQ(sum2, sum);

    int expect = 0;
    for(auto e = map1.begin(); e; e++)
        expect += e->cast<myNode>()->number;

    ASSERT_EQ(sum, expect);
}

TEST_F(nchainTest, testLinkArrayAndChain) {
    nmap map1;
    map1.add("1", new myNode(1));
    map1.add("2", new myNode(2));
    nchain chn(map1);
    ASSERT_EQ(chn.len(), 2);
    ASSERT_FALSE(chn.getNext());

    nchain chn2;
    chn2.add("3", new myNode(3));
    nmap map2;
    map2.add("4", new myNode(4));
    map2.add("5", new myNode(5));
    chn2.link(chn2.wrap(map2));
    ASSERT_EQ(chn2.len(), 3);
    ASSERT_TRUE(chn2.getNext());
    ASSERT_EQ(&chn2.getNext()->getContainer(), &map2);

    chn.link(chn2);
    int cnt = 5;
    ASSERT_EQ(chn.len(), cnt);

    for(int n = 1; n <= 5; n++) {
        std::string key = std::to_string(n);
        ASSERT_TRUE(chn.in(key));
        ASSERT_EQ(chn[key].cast<myNode>()->number, n);
    }
}

TEST_F(nchainTest, testChainCopy) {
    tnchain<float, myNode> chn1;
    chn1.add(1.0, new myNode(1));
    chn1.add(2.0, new myNode(2));
    ASSERT_EQ(chn1.len(), 2);

    tnchain<float, myNode> chn2;
    chn2.add(3.0, new myNode(3));
    chn2.add(4.0, new myNode(4));
    ASSERT_EQ(chn2.len(), 2);

    // current status: chn1 -> chn2
    chn1.link(chn2);
    ASSERT_EQ(chn1.len(), 4);

    // current status: chn1   -> chn2
    //                 cloned -> chn2
    tstr<tnchain<float, myNode>> cloned(chn1.cloneChain());
    ASSERT_EQ(cloned->len(), 4);

    tnchain<float, myNode> chn3;
    chn3.add(5.0, new myNode(5));
    chn3.add(6.0, new myNode(6));
    ASSERT_EQ(chn3.len(), 2);

    // current status: chn1 -> chn2
    //                 chn3 -> cloned -> chn2
    chn3.link(*cloned);
    ASSERT_EQ(cloned->len(), 4);
    ASSERT_EQ(chn3.len(), 6);

    // current status: chn1 -> chn2
    //                 chn3 -> cloned -> chn2
    //              cloned2 -> cloned
    tstr<tnchain<float, myNode>> cloned2(chn3.cloneChain(cloned.get()));
    ASSERT_EQ(cloned2->len(), 4);

    // current status: chn1 -> chn2(size=3)
    //                 chn3 -> cloned -> chn2(size=3)
    //              cloned2 -> cloned -> chn1 -> chn2(size=3)
    ASSERT_TRUE(cloned2->getNext());
    cloned2->getNext()->link(chn1);
    ASSERT_EQ(cloned2->len(), 8);

    chn2.add(7.0, new myNode(7));
    ASSERT_EQ(chn1.len(), 5);
    ASSERT_EQ(chn3.len(), 7);
    ASSERT_EQ(cloned2->len(), 9);


    std::vector<float> tray;
    for(auto e = cloned2->begin(); e; ++e) {
        float& key = e.getKey() OR_ASSERT(key);
        ASSERT_EQ(key, (float) e->cast<myNode>()->number);
        tray.push_back(key);
    }


    for(int n = 1; n <= 7; n++)
        ASSERT_TRUE(vectorHas(tray, n));
}

TEST_F(nchainTest, testDeepChainAddDel) {
    tnchain<float, myNode> chn1;
    chn1.add(1.0, new myNode(1));
    chn1.add(2.0, new myNode2(2));

    tnchain<float, myNode> chn2;
    chn2.add(3.0, new myNode2(3));
    chn2.add(4.0, new myNode(4));
    chn1.link(chn2); // chn1 -> chn2

    tstr<tnchain<float, myNode>> root(chn1.cloneChain());
    // chn1 ---> chn2
    // root --|

    tnchain<float, myNode> chn3;
    chn3.add(5.0, new myNode2(5));
    chn3.add(6.0, new myNode(6));

    tnchain<float, myNode>& tail = root->getTail() OR_ASSERT(tail); // tail is chn2 from root.
    ASSERT_EQ(tail.len(), 2);
    ASSERT_NE(&tail, &chn2);
    ASSERT_EQ(tail.getContainer(), chn2.getContainer());

    // current graph: root -> chn2 -> chn3
    //                chn1 -> chn2
    tail.link(chn3);
    ASSERT_EQ(root->len(), 6);
    ASSERT_EQ(chn1.len(), 4);

    auto e = root->begin();
    e = e + 2;
    ASSERT_TRUE(e.getVal());
    ASSERT_EQ(root->get(6.0)->number, 6);

    // current graph: root -> chn2(size=1) -> chn3
    //                chn1 -> chn2(size=1)
    root->del(3.0);
    ASSERT_EQ(root->len(), 5);
    ASSERT_EQ(chn1.len(), 3);

    {
        float expects[] = {1.0, 2.0, 4.0, 5.0, 6.0};
        ASSERT_TRUE(isMyNodesHasEqualIntArray<>(*root, expects, 5));
    }

    {
        float expects[] = {2.0, 5.0};
        ASSERT_TRUE(isMyNodesHasEqualIntArray<myNode2>(*root, expects, 2));
    }

    root->del(root->begin() + 1, root->last()); // last element will remain.
    ASSERT_EQ(root->len(), 2);

    {
        float expects[] = {1.0, 6.0};
        ASSERT_TRUE(isMyNodesHasEqualIntArray<>(*root, expects, 2));
    }

    bool found = false;
    for(const auto& elem: *root)
        if(elem.isSub<myNode2>()) {
            found = true;
            break;
        }
    ASSERT_FALSE(found);
}

TEST_F(nchainTest, delWhileIteration) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    m.add("apple", new nInt(3));
    m.add("banana", new nInt(4));
    m.add("meat", new nInt(5));
    m.add("banana", new nInt(6));
    m.add("meat", new nInt(7));
    m.add("banana", new nInt(8));

    for(auto e = m.begin(); e;)
        if(*e.getKey() == "banana") m.del(e++);
        else ++e;

    ASSERT_EQ(m.len(), 4);
    ASSERT_EQ(*m.get("apple")->cast<int>(), 3);
    ASSERT_EQ(m.getAll("meat").len(), 3);
}

TEST_F(nchainTest, chainMultipleLinkAndCheckPrev) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("mango", new nInt(4));
    nchain m3;
    m3.add("pineapple", new nInt(5));
    m3.add("melon", new nInt(6));

    ASSERT_TRUE(m.link(m2));
    ASSERT_TRUE(m2.link(m3));
    ASSERT_FALSE(m3.link(m3)); // exception handling for recursive linkage.

    {
        nchain* expects[] = {&m, &m2, &m3};
        int n = 0;
        for(nchain* e = &m; e; e = e->getNext())
            ASSERT_EQ(e, expects[n++]);
    }

    {
        nchain* expects[] = {&m3, &m2, &m};
        int n = 0;
        for(nchain* e = &m3; e; e = e->getPrev())
            ASSERT_EQ(e, expects[n++]);
    }
}

TEST_F(nchainTest, unlinkSuddenlyMiddleOfMultipleChain) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("mango", new nInt(4));
    nchain m3;
    m3.add("pineapple", new nInt(5));
    m3.add("melon", new nInt(6));

    ASSERT_TRUE(m.link(m2));
    ASSERT_TRUE(m2.link(m3)); // m1 -> m2 -> m3

    {
        nchain* expects[] = {&m, &m2, &m3};
        int n = 0;
        for(nchain* e = &m; e; e = e->getNext())
            ASSERT_EQ(e, expects[n++]);
    }

    m2.unlink(); // m1 -> m2 -> X

    {
        nchain* expects[] = {&m, &m2};
        int n = 0;
        for(nchain* e = &m; e; e = e->getNext())
            ASSERT_EQ(e, expects[n++]);
        ASSERT_FALSE(m3.getPrev());
    }
}

TEST_F(nchainTest, iterateForKey) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    m.add("apple", new nInt(3));
    m.add("banana", new nInt(4));
    m.add("meat", new nInt(5));
    m.add("banana", new nInt(6));
    m.add("meat", new nInt(7));
    m.add("banana", new nInt(8));

    {
        auto e = m.iterate("apple");
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "apple");
        nInt& val = *e->cast<nInt>();
        ASSERT_EQ(val.get(), 3);
    }

    {
        auto e = m.iterate("banana");
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "banana");
        nInt& val = *e->cast<nInt>();
        ASSERT_EQ(val.get(), 2);
    }

    {
        auto e = m.riterate("banana");
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "banana");
        nInt& val = *e->cast<nInt>();
        ASSERT_EQ(val.get(), 8);
    }
}

TEST_F(nchainTest, iterateMultipleChain) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("banana", new nInt(4));

    m2.link(m); // apple --> banana --> meat --> banana

    std::string expects[] = {"apple", "banana", "meat", "banana"};
    int n = 0;
    for(auto e = m2.begin(); e; ++e)
        ASSERT_EQ(*e.getKey(), expects[n++]);
}

TEST_F(nchainTest, iterateMultipleChainReversedWay) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("banana", new nInt(4));

    m2.link(m); // apple --> banana --> meat --> banana

    std::string expects[] = {"banana", "meat", "banana", "apple"};
    int n = 0;
    for(auto e = m2.rbegin(); e; ++e)
        ASSERT_EQ(*e.getKey(), expects[n++]);
}

TEST_F(nchainTest, iterateForKeyInMultipleChain) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("banana", new nInt(4));

    m2.link(m); // apple --> banana --> meat --> banana

    {
        auto e = m2.iterate("apple");
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "apple");
        nInt& val = *e->cast<nInt>();
        ASSERT_EQ(val.get(), 3);
    }

    {
        auto e = m2.iterate("banana");
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "banana");
        nInt& val = *e->cast<nInt>();
        ASSERT_EQ(val.get(), 4);

        ++e;
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "banana");
        nInt& val2 = *e->cast<nInt>();
        ASSERT_EQ(val2.get(), 2);
    }

    {
        auto e = m2.riterate("banana");
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "banana");
        nInt& val = *e->cast<nInt>();
        ASSERT_EQ(val.get(), 2);

        ++e;
        ASSERT_FALSE(e.isEnd());
        ASSERT_EQ(*e.getKey(), "banana");
        nInt& val2 = *e->cast<nInt>();
        ASSERT_EQ(val2.get(), 4);
    }
}

TEST_F(nchainTest, linkFirstAddSecond) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("banana", new nInt(3));

    m.link(m2); // meat -> banan -> banana
    ASSERT_EQ(m.len(), 3);

    {
        std::string expectKeys[] = {"meat", "banana", "banana"};
        int n = 0;
        for(auto e = m.begin(); e; ++e)
            ASSERT_EQ(*e.getKey(), expectKeys[n++]);
    }

    m2.add("apple", *new nInt(4));
    m2.add("mango", *new nInt(5));

    ASSERT_EQ(m.len(), 5);
    {
        std::string expectKeys[] = {"meat", "banana", "banana", "apple", "mango"};
        int n = 0;
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n++]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), n);
        }
    }
}

TEST_F(nchainTest, linkREndAndAddingElemCanAffect) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("banana", new nInt(4));

    m.link(m2.rend()); // meat -> banana -> banana -> apple
    ASSERT_EQ(m.len(), 4);

    {
        std::string expectKeys[] = {"meat", "banana", "banana", "apple"};
        int expects[] = {1, 2, 4, 3};
        int n = 0;
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }

    // this adding 1 element at the end of m2.
    // m has linked to `rend` of m2.
    // in this case, `m` still can see m2's new element.
    m2.add("melon", new nInt(5));
    ASSERT_EQ(m.len(), 5);
    {
        std::string expectKeys[] = {"meat", "banana", "melon", "banana", "apple"};
        int expects[] = {1, 2, 5, 4, 3};
        int n = 0;
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }
}

TEST_F(nchainTest, linkReversedKeySpecificAndAddingElemCantAffect) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("banana", new nInt(4));
    m.link(m2.riterate(0, false)); // it's not boundary!
    // meat -> banana -> banana -> apple

    {
        std::string expectKeys[] = {"meat", "banana", "banana", "apple"};
        int expects[] = {1, 2, 4, 3};
        int n = 0;
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }

    m2.add("melon", new nInt(5));

    {
        std::string expectKeys[] = {"meat", "banana", "banana", "apple"};
        int expects[] = {1, 2, 4, 3};
        int n = 0;
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }
}

TEST_F(nchainTest, linkChainsButMiddleOfOneIsReversed) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("banana", new nInt(4));
    nchain m3;
    m3.add("mango", new nInt(5));
    m3.add("melon", new nInt(6));

    m.link(m2.rbegin());
    m2.link(m3); // m -> m2(reversed) -> m3
    // meat -> banana -> banana -> apple -> mango -> melon

    {
        std::string expectKeys[] = {"meat", "banana", "banana", "apple", "mango", "melon"};
        int expects[] = {1, 2, 4, 3, 5, 6};
        int n = 0;
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }

    {
        std::string expectKeys[] = {"melon", "mango", "apple", "banana", "banana", "meat"};
        int expects[] = {6, 5, 3, 4, 2, 1};
        int n = 0;
        for(auto e = m.rbegin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }
}

TEST_F(nchainTest, complexLinkTest) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    m2.add("apple", new nInt(3));
    m2.add("banana", new nInt(4));
    m2.add("pineapple", new nInt(5));
    nchain m3;
    m3.add("mango", new nInt(6));
    m3.add("melon", new nInt(7));

    m.link(m2.rbegin() + 1); // banana
    m2.link(m3.begin() + 1); // melon
    // meat -> banana -> banana -> apple -> melon

    m2.add("watermelon", new nInt(8));
    m3.add("strawberry", new nInt(9));
    m.add("pumpkin", new nInt(10));

    // [meat -> banana -> pumpkin] -> [banana -> apple] -> [melon -> strawberry]

    {
        std::string expectKeys[] = {"meat", "banana", "pumpkin", "banana", "apple", "melon", "strawberry"};
        int expects[] = {1, 2, 10, 4, 3, 7, 9};
        int n = 0;
        ASSERT_EQ(m.len(), 7);
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }

    {
        std::string expectKeys[] = {"strawberry", "melon", "apple", "banana", "pumpkin", "banana", "meat"};
        int expects[] = {9, 7, 3, 4, 10, 2, 1};
        int n = 0;
        for(auto e = m.rbegin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e.getVal()->cast<nint>(), expects[n++]);
        }
    }
}

TEST_F(nchainTest, toLinkEmptyChainIsValid) {
    nchain m;
    m.add("meat", new nInt(1));
    m.add("banana", new nInt(2));
    nchain m2;
    nchain m3;
    m3.add("mango", new nInt(3));
    m.link(m2);
    m2.link(m3);
    m3.add("melon", new nInt(4));

    ASSERT_EQ(m.len(), 4);

    {
        std::string expectKeys[] = {"meat", "banana", "mango", "melon"};
        int expects[] = {1, 2, 3, 4};
        int n = 0;
        for(auto e = m.begin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e->cast<nint>(), expects[n++]);
        }
    }

    {
        std::string expectKeys[] = {"melon", "mango", "banana", "meat"};
        int expects[] = {4, 3, 2, 1};
        int n = 0;
        for(auto e = m.rbegin(); e; ++e) {
            ASSERT_EQ(*e.getKey(), expectKeys[n]);
            ASSERT_EQ(*e->cast<nint>(), expects[n++]);
        }
    }
}

TEST_F(nchainTest, emptedChainLinkEmptyChain) {
    nchain m;
    nchain m2;
    ASSERT_FALSE(m.link(m));
    ASSERT_TRUE(m.link(m2));
    ASSERT_EQ(m.len(), 0);

    m2.add("meat", new nInt(1));
    ASSERT_EQ(m2.len(), 1);
    ASSERT_EQ(m.len(), 1);

    auto e = m.begin();
    ASSERT_EQ(*e.getKey(), "meat");
    ASSERT_EQ(*e->cast<nint>(), 1);
}
