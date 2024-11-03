#include "TestEzGraph.h"
#include <ezlibs/ezGraph.hpp>
#include <ezlibs/ezCTest.hpp>

class TestNode;
typedef std::shared_ptr<TestNode> TestNodePtr;
typedef std::weak_ptr<TestNode> TestNodeWeak;

struct TestNodeDatas : ez::NodeDatas {
    std::string mode;

    TestNodeDatas() = default;

    explicit TestNodeDatas(std::string vName, std::string vType, std::string mode) : ez::NodeDatas(std::move(vName), std::move(vType)), mode(std::move(mode)) {}
};

class TestNode : public ez::Node {
public:
    static TestNodePtr create(const TestNodeDatas &vNodeDatas) {
        auto node_ptr = std::make_shared<TestNode>(vNodeDatas);
        node_ptr->m_setThis(node_ptr);
        return node_ptr;
    }

    static ez::RetCodes connectSlots(const ez::SlotWeak &vFrom, const ez::SlotWeak &vTo) { return m_connectSlots(vFrom, vTo); }

    template <typename T, typename = std::enable_if<std::is_base_of<TestNodeDatas, T>::value>>
    explicit TestNode(const T &vDatas) : Node(std::make_shared<T>(vDatas)) {}

    template <typename U, typename = std::enable_if<std::is_base_of<ez::Node, U>::value>>
    std::weak_ptr<U> createChildNode(const TestNodeDatas &vNodeDatas) {
        auto node_ptr = std::make_shared<U>(vNodeDatas);
        node_ptr->m_setThis(node_ptr);
        if (m_addNode(node_ptr) != ez::RetCodes::SUCCESS) {
            node_ptr.reset();
        }
        return node_ptr;
    }

    template <typename U, typename = std::enable_if<std::is_base_of<ez::Slot, U>::value>>
    std::weak_ptr<U> addSlot(const ez::SlotDatas &vSlotDatas) {
        auto slot_ptr = std::make_shared<U>(vSlotDatas);
        if (m_addSlot(slot_ptr) != ez::RetCodes::SUCCESS) {
            slot_ptr.reset();
        }
        return slot_ptr;
    }
};

class NodeNumber;
typedef std::shared_ptr<NodeNumber> NodeNumberPtr;
typedef std::weak_ptr<NodeNumber> NodeNumberWeak;

class NodeNumber : public TestNode {
public:
    static NodeNumberPtr create(const TestNodeDatas &vNodeDatas) {
        auto node_ptr = std::make_shared<NodeNumber>(vNodeDatas);
        node_ptr->m_setThis(node_ptr);
        return node_ptr;
    }

private:
    float m_Value = 0.0f;

public:
    explicit NodeNumber(const TestNodeDatas &vDatas) : TestNode(vDatas) {}

    void setValue(const float vValue) { m_Value = vValue; }
    float getValue() const { return m_Value; }
};

class NodeOpAdd;
typedef std::shared_ptr<NodeOpAdd> NNodeOpAddPtr;
typedef std::weak_ptr<NodeOpAdd> NodeOpAddWeak;

class NodeOpAdd : public NodeNumber {
public:
    static NNodeOpAddPtr create(const TestNodeDatas &vNodeDatas) {
        auto node_ptr = std::make_shared<NodeOpAdd>(vNodeDatas);
        node_ptr->m_setThis(node_ptr);
        return node_ptr;
    }

    explicit NodeOpAdd(const TestNodeDatas &vDatas) : NodeNumber(vDatas) {}

    ez::RetCodes eval(const size_t vFrame, const ez::SlotWeak &vFrom) final {
        auto ret = ez::RetCodes::SUCCESS;
        float sum = 0.0f;
        auto outSlotPtr = vFrom.lock();
        if (outSlotPtr != nullptr) {
            auto outNodePtr = std::dynamic_pointer_cast<NodeOpAdd>(outSlotPtr->getParentNode().lock());
            if (outNodePtr != nullptr) {
                for (const auto &inSlotPtr : m_getInputsRef()) {
                    if (inSlotPtr != nullptr) {
                        for (const auto &conSlot : inSlotPtr->m_getConnectedSlots()) {
                            auto conSlotPtr = conSlot.lock();
                            if (conSlotPtr != nullptr) {
                                auto inNodePtr = std::dynamic_pointer_cast<NodeNumber>(conSlotPtr->getParentNode().lock());
                                if (inNodePtr != nullptr) {
                                    sum += inNodePtr->getValue();
                                } else {
                                    ret = ez::RetCodes::FAILED_SLOT_PTR_NULL;
                                    break;
                                }
                            }
                        }
                    }
                }
                outNodePtr->setValue(sum);
                ez::EvalDatas datas;
                datas.frame = vFrame;
                outSlotPtr->setLastEvaluatedDatas(datas);
            } else {
                ret = ez::RetCodes::FAILED_SLOT_PTR_NULL;
            }
        } else {
            ret = ez::RetCodes::FAILED_SLOT_PTR_NULL;
        }
        return ret;
    }
};

////////////////////////////////////////////////////////////////////////////
////  Graph ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

bool TestEzGraph_Building() {
    return true;
}

bool TestEzGraph_Evaluation() {
    ez::SlotDatas output_slot_datas;
    output_slot_datas.dir = ez::SlotDir::OUTPUT;
    ez::SlotDatas input_slot_datas;
    input_slot_datas.dir = ez::SlotDir::INPUT;

    auto graphPtr = TestNode::create({});  // a graph is just a node who have node childs
    CTEST_ASSERT(graphPtr != nullptr);

    auto nodaNumA = graphPtr->createChildNode<NodeNumber>(TestNodeDatas("nodaNumA", "NodaNumber", "modeA"));
    CTEST_ASSERT(nodaNumA.expired() == false);
    CTEST_ASSERT(nodaNumA.lock() != nullptr);
    auto nodeNumASlotOutput = nodaNumA.lock()->addSlot<ez::Slot>(output_slot_datas);
    CTEST_ASSERT(nodeNumASlotOutput.expired() == false);
    CTEST_ASSERT(nodeNumASlotOutput.lock() != nullptr);
    nodaNumA.lock()->setValue(5.0f);
    CTEST_ASSERT(nodaNumA.lock()->getValue() == 5.0f);
    CTEST_ASSERT(nodaNumA.lock()->getDatas<TestNodeDatas>().mode == "modeA");
    CTEST_ASSERT(nodaNumA.lock()->getDatas<ez::NodeDatas>().name == "nodaNumA");
    CTEST_ASSERT(nodaNumA.lock()->getDatas<ez::NodeDatas>().type == "NodaNumber");
    nodaNumA.lock()->getDatasRef<TestNodeDatas>().mode = "modeABis";
    CTEST_ASSERT(nodaNumA.lock()->getDatas<TestNodeDatas>().mode == "modeABis");

    auto nodaNumB = graphPtr->createChildNode<NodeNumber>(TestNodeDatas("nodaNumB", "NodaNumber", "modeB"));
    CTEST_ASSERT(nodaNumB.expired() == false);
    CTEST_ASSERT(nodaNumB.lock() != nullptr);
    auto nodeNumBSlotOutput = nodaNumB.lock()->addSlot<ez::Slot>(output_slot_datas);
    CTEST_ASSERT(nodeNumBSlotOutput.expired() == false);
    CTEST_ASSERT(nodeNumBSlotOutput.lock() != nullptr);
    nodaNumB.lock()->setValue(200.0f);
    CTEST_ASSERT(nodaNumB.lock()->getValue() == 200.0f);
    CTEST_ASSERT(nodaNumB.lock()->getDatas<TestNodeDatas>().mode == "modeB");
    CTEST_ASSERT(nodaNumB.lock()->getDatas<ez::NodeDatas>().name == "nodaNumB");
    CTEST_ASSERT(nodaNumB.lock()->getDatas<ez::NodeDatas>().type == "NodaNumber");
    nodaNumB.lock()->getDatasRef<TestNodeDatas>().mode = "modeBBis";
    CTEST_ASSERT(nodaNumB.lock()->getDatas<TestNodeDatas>().mode == "modeBBis");

    auto nodaOpAdd = graphPtr->createChildNode<NodeOpAdd>(TestNodeDatas("nodaOpAdd", "NodeOpAdd", "modeAdd"));
    CTEST_ASSERT(nodaOpAdd.expired() == false);
    CTEST_ASSERT(nodaOpAdd.lock() != nullptr);
    auto nodeOpAddSlotInputA = nodaOpAdd.lock()->addSlot<ez::Slot>(input_slot_datas);
    CTEST_ASSERT(nodeOpAddSlotInputA.expired() == false);
    CTEST_ASSERT(nodeOpAddSlotInputA.lock() != nullptr);
    auto nodeOpAddSlotInputB = nodaOpAdd.lock()->addSlot<ez::Slot>(input_slot_datas);
    CTEST_ASSERT(nodeOpAddSlotInputB.expired() == false);
    CTEST_ASSERT(nodeOpAddSlotInputB.lock() != nullptr);
    auto nodeOpAddSlotOutput = nodaOpAdd.lock()->addSlot<ez::Slot>(output_slot_datas);
    CTEST_ASSERT(nodeOpAddSlotOutput.expired() == false);
    CTEST_ASSERT(nodeOpAddSlotOutput.lock() != nullptr);
    CTEST_ASSERT(graphPtr->connectSlots(nodeNumASlotOutput.lock(), nodeOpAddSlotInputA.lock()) == ez::RetCodes::SUCCESS);
    CTEST_ASSERT(graphPtr->connectSlots(nodeNumBSlotOutput.lock(), nodeOpAddSlotInputB.lock()) == ez::RetCodes::SUCCESS);
    CTEST_ASSERT(nodaOpAdd.lock()->eval(10, nodeOpAddSlotOutput.lock()) == ez::RetCodes::SUCCESS);
    CTEST_ASSERT(nodaOpAdd.lock()->getValue() == 205.0f);
    CTEST_ASSERT(nodeOpAddSlotOutput.lock()->getLastEvaluatedDatas().frame == 10);
    CTEST_ASSERT(nodaOpAdd.lock()->getDatas<TestNodeDatas>().mode == "modeAdd");
    CTEST_ASSERT(nodaOpAdd.lock()->getDatas<ez::NodeDatas>().name == "nodaOpAdd");
    CTEST_ASSERT(nodaOpAdd.lock()->getDatas<ez::NodeDatas>().type == "NodeOpAdd");
    nodaOpAdd.lock()->getDatasRef<TestNodeDatas>().mode = "modeAddBis";
    CTEST_ASSERT(nodaOpAdd.lock()->getDatas<TestNodeDatas>().mode == "modeAddBis");

    return true;
}

////////////////////////////////////////////////////////////////////////////
//// ENTRY POINT ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

#define IfTestExist(v)            \
    if (vTest == std::string(#v)) \
    return v()

#define IfTestCollectionExist(v, str)        \
    if (vTest.find(#v) != std::string::npos) \
    return v(str)

bool TestEzGraph(const std::string& vTest) {
    IfTestExist(TestEzGraph_Building);
    else IfTestExist(TestEzGraph_Evaluation);

    return false;
}