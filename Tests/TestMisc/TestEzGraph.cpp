#include "TestEzGraph.h"
#include "TestEzGraphSlot.h"
#include "TestEzGraphNode.h"
#include <ezLibs/ezGraph.hpp>

class TestSlot;
typedef std::shared_ptr<TestSlot> TestSlotPtr;
typedef std::weak_ptr<TestSlot> TestSlotWeak;

class TestSlot : public ez::Node {
public:
    static TestSlotPtr create(const ez::NodeDatas& vNodeDatas) {
        auto node_ptr = std::make_shared<TestSlot>();
        if (!node_ptr->initNode(vNodeDatas, node_ptr)) {
            node_ptr.reset();
        }
        return node_ptr;
    }

    template <typename U>
    TestSlotWeak createChildNode(const ez::NodeDatas& vNodeDatas) {
        auto node_ptr = std::make_shared<U>();
        if (!node_ptr->initNode(vNodeDatas, node_ptr)) {
            node_ptr.reset();
        } else {
            if (m_addNode(node_ptr) != ez::RetCodes::SUCCESS) {
                node_ptr.reset();
            }
        }
        return node_ptr;
    }
};

class TestNode;
typedef std::shared_ptr<TestNode> TestNodePtr;
typedef std::weak_ptr<TestNode> TestNodeWeak;

class TestNode : public ez::Node {
public:
    static TestNodePtr create(const ez::NodeDatas& vNodeDatas) {
        auto node_ptr = std::make_shared<TestNode>();
        if (!node_ptr->initNode(vNodeDatas, node_ptr)) {
            node_ptr.reset();
        }
        return node_ptr;
    }

public:
    template <typename U, typename = std::enable_if<std::is_base_of<ez::Node, U>::value>>
    std::weak_ptr<U> createChildNode(const ez::NodeDatas& vNodeDatas) {
        auto node_ptr = std::make_shared<U>();
        if (!node_ptr->initNode(vNodeDatas, node_ptr)) {
            node_ptr.reset();
        } else {
            if (m_addNode(node_ptr) != ez::RetCodes::SUCCESS) {
                node_ptr.reset();
            }
        }
        return node_ptr;
    }
    template <typename U, typename = std::enable_if<std::is_base_of<ez::Slot, U>::value>>
    std::weak_ptr<U> addSlot(const ez::SlotDatas& vSlotDatas) {
        auto slot_ptr = std::make_shared<U>();
        if (!slot_ptr->initSlot(vSlotDatas, slot_ptr)) {
            slot_ptr.reset();
        } else {
            if (m_addSlot(slot_ptr) != ez::RetCodes::SUCCESS) {
                slot_ptr.reset();
            }
        }
        return slot_ptr;
    }

public:
    ez::RetCodes connectSlots(ez::SlotWeak vFrom, ez::SlotWeak vTo) { return m_connectSlots(vFrom, vTo); }
};

class NodeNumber;
typedef std::shared_ptr<NodeNumber> NodeNumberPtr;
typedef std::weak_ptr<NodeNumber> NodeNumberWeak;

class NodeNumber : public TestNode {
public:
    static NodeNumberPtr create(const ez::NodeDatas& vNodeDatas) {
        auto node_ptr = std::make_shared<NodeNumber>();
        if (!node_ptr->initNode(vNodeDatas, node_ptr)) {
            node_ptr.reset();
        }
        return node_ptr;
    }

private:
    float m_Value = 0.0f;

public:
    void setValue(const float vValue) { m_Value = vValue; }
    float getValue() { return m_Value; }
};

class NodeOpAdd;
typedef std::shared_ptr<NodeOpAdd> NNodeOpAddPtr;
typedef std::weak_ptr<NodeOpAdd> NodeOpAddWeak;

class NodeOpAdd : public NodeNumber {
public:
    static NNodeOpAddPtr create(const ez::NodeDatas& vNodeDatas) {
        auto node_ptr = std::make_shared<NodeOpAdd>();
        if (!node_ptr->initNode(vNodeDatas, node_ptr)) {
            node_ptr.reset();
        }
        return node_ptr;
    }

    ez::RetCodes eval(const size_t vFrame, const ez::SlotWeak& vFrom) final {
        auto ret = ez::RetCodes::SUCCESS;
        float sum = 0.0f;
        auto outSlotPtr = vFrom.lock();
        if (outSlotPtr != nullptr) {
            auto outNodePtr = std::dynamic_pointer_cast<NodeOpAdd>(outSlotPtr->getParentNode().lock());
            if (outNodePtr != nullptr) {
                for (auto inSlotPtr : m_getInputsRef()) {
                    if (inSlotPtr != nullptr) {
                        for (const auto& conSlot : inSlotPtr->m_getConnectedSlots()) {
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

#define CTEST_ASSERT(cond) \
    if (!(cond))           \
    return false

/*bool TestEzGraph_Uuids() {
    ez::UuidBank bank;
    CTEST_ASSERT(bank.add() == 1);
    CTEST_ASSERT(bank.add() == 2);
    CTEST_ASSERT(bank.add() == 3);
    CTEST_ASSERT(bank.del(2) == true);
    CTEST_ASSERT(bank.del(4) == false);
    CTEST_ASSERT(bank.add() == 2);
    CTEST_ASSERT(bank.add() == 4);
    return true;
}*/

bool TestEzGraph_Building() {
    return true;
}

bool TestEzGraph_Evaluation() {
    ez::NodeDatas node_datas;
    ez::SlotDatas output_slot_datas;
    output_slot_datas.dir = ez::SlotDir::OUTPUT;
    ez::SlotDatas input_slot_datas;
    input_slot_datas.dir = ez::SlotDir::INPUT;

    auto graphPtr = TestNode::create({});  // a graph is just a node who have node childs
    CTEST_ASSERT(graphPtr != nullptr);

    auto nodaNumA = graphPtr->createChildNode<NodeNumber>(node_datas);
    CTEST_ASSERT(nodaNumA.expired() == false);
    CTEST_ASSERT(nodaNumA.lock() != nullptr);
    auto nodeNumASlotOutput = nodaNumA.lock()->addSlot<ez::Slot>(output_slot_datas);
    CTEST_ASSERT(nodeNumASlotOutput.expired() == false);
    CTEST_ASSERT(nodeNumASlotOutput.lock() != nullptr);
    nodaNumA.lock()->setValue(5.0f);
    CTEST_ASSERT(nodaNumA.lock()->getValue() == 5.0f);

    auto nodaNumB = graphPtr->createChildNode<NodeNumber>(node_datas);
    CTEST_ASSERT(nodaNumB.expired() == false);
    CTEST_ASSERT(nodaNumB.lock() != nullptr);
    auto nodeNumBSlotOutput = nodaNumB.lock()->addSlot<ez::Slot>(output_slot_datas);
    CTEST_ASSERT(nodeNumBSlotOutput.expired() == false);
    CTEST_ASSERT(nodeNumBSlotOutput.lock() != nullptr);
    nodaNumB.lock()->setValue(200.0f);
    CTEST_ASSERT(nodaNumB.lock()->getValue() == 200.0f);

    auto nodaOpAdd = graphPtr->createChildNode<NodeOpAdd>(node_datas);
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
    IfTestCollectionExist(TestEzGraphSlot, vTest);
    else IfTestCollectionExist(TestEzGraphNode, vTest);

    IfTestExist(TestEzGraph_Building);
    else IfTestExist(TestEzGraph_Evaluation);

    return false;
}
