#pragma once

#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <functional>

namespace ez {

/////////////////////////////////////
///// Utils /////////////////////////
/////////////////////////////////////

namespace Utils {

// if the shared_ptr exit in the container return the iterator
template <typename T>
typename std::vector<std::shared_ptr<T>>::iterator  //
isSharedPtrExistInVector(std::shared_ptr<T> vPtr, std::vector<std::shared_ptr<T>>& vContainer) {
    auto ret = vContainer.end();
    if (vPtr != nullptr) {
        ret = vContainer.begin();
        for (; ret != vContainer.end(); ++ret) {
            if (*ret == vPtr) {
                break;
            }
        }
    }
    return ret;
}

// if the weak_ptr exit in the container return the iterator
template <typename T>
typename std::vector<std::weak_ptr<T>>::iterator  //
isWeakPtrExistInVector(std::weak_ptr<T> vWeak, std::vector<std::weak_ptr<T>>& vContainer) {
    auto ret = vContainer.end();
    if (!vWeak.expired()) {
        auto ptr = vWeak.lock();
        ret = vContainer.begin();
        for (; ret != vContainer.end(); ++ret) {
            if (ret->lock() == ptr) {
                break;
            }
        }
    }
    return ret;
}

}  // namespace Utils

/////////////////////////////////////
///// DEFS //////////////////////////
/////////////////////////////////////

enum class SlotDir {  //
    INPUT = 0,
    OUTPUT,
    Count
};

enum class RetCodes {  //
    SUCCESS = 0,
    FAILED,
    FAILED_SLOT_PTR_NULL,
    FAILED_SLOT_ALREADY_EXIST,
    FAILED_SLOT_NOT_FOUND,
    FAILED_NODE_PTR_NULL,
    FAILED_NODE_ALREADY_EXIST,
    FAILED_NODE_NOT_FOUND,
    FAILED_NODE_NO_FUNCTOR,
    Count
};

class Slot;
typedef std::shared_ptr<Slot> SlotPtr;
typedef std::weak_ptr<Slot> SlotWeak;

class Node;
typedef std::shared_ptr<Node> NodePtr;
typedef std::weak_ptr<Node> NodeWeak;
typedef std::function<RetCodes(const size_t, const std::vector<SlotPtr>&, const SlotWeak&)> NodeFunctor;

class Graph;
typedef std::shared_ptr<Graph> GraphPtr;
typedef std::weak_ptr<Graph> GraphWeak;

class Node;
typedef std::shared_ptr<Node> NodePtr;
typedef std::weak_ptr<Node> NodeWeak;

typedef void* UserDatas;

/////////////////////////////////////
///// SLOT //////////////////////////
/////////////////////////////////////

struct SlotDatas {
    std::string m_Name;
    std::string m_Type;
    UserDatas m_UserDatas = nullptr;
    SlotDir m_Dir = SlotDir::INPUT;
};

struct EvalDatas {
    size_t frame = 0U;
};

class Slot {
public:
    static SlotPtr create(const SlotDatas& vDatas) {
        auto ret = std::make_shared<Slot>();
        ret->m_This = ret;
        if (!ret->init(vDatas)) {
            ret.reset();
        }
        return ret;
    }

private:
    SlotWeak m_This;
    NodeWeak m_ParentNode;
    SlotDatas m_Datas;
    std::vector<SlotWeak> m_ConnectedSlots;
    EvalDatas m_LastEvaluatedDatas;

public:
    virtual bool init(const SlotDatas& vDatas) {
        m_Datas = vDatas;
        return true;
    }
    virtual void unit() {
        m_Datas = {};
        m_ConnectedSlots.clear();
    }
    RetCodes connectSlot(SlotWeak vSlot) {
        RetCodes ret = RetCodes::FAILED_SLOT_PTR_NULL;
        if (!vSlot.expired()) {
            m_ConnectedSlots.push_back(vSlot);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    RetCodes disconnectSlot(SlotWeak vSlot) {
        RetCodes ret = RetCodes::FAILED_SLOT_ALREADY_EXIST;
        auto it = Utils::isWeakPtrExistInVector(vSlot, m_ConnectedSlots);
        if (it != m_ConnectedSlots.end()) {
            m_ConnectedSlots.erase(it);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    void disconnect() {
        m_ConnectedSlots.clear();
    }
    const SlotDatas& getDatas() {
        return m_Datas;
    }
    void setParentNode(NodeWeak vNodeWeak) {
        m_ParentNode = vNodeWeak;
    }
    NodeWeak getParentNode() {
        return m_ParentNode;
    }
    const std::vector<SlotWeak>& getConnectedSlots() {
        return m_ConnectedSlots;
    }
    void setLastEvaluatedDatas(const EvalDatas vUserDatas) {
        m_LastEvaluatedDatas = vUserDatas;
    }
    const EvalDatas& getLastEvaluatedDatas() {
        return m_LastEvaluatedDatas;
    }
};

/////////////////////////////////////
///// NODE //////////////////////////
/////////////////////////////////////

struct NodeDatas {
    std::string name;
    std::string type;
    UserDatas userDatas = nullptr;
    NodeFunctor functor = nullptr;
};

class Node {
public:
    static NodePtr create(const NodeDatas& vNodeDatas) {
        auto ret = std::make_shared<Node>();
        ret->m_This = ret;
        if (!ret->init(vNodeDatas)) {
            ret.reset();
        }
        return ret;
    }

private:
    NodeWeak m_This;
    std::vector<SlotPtr> m_Inputs;
    std::vector<SlotPtr> m_Outputs;
    NodeDatas m_NodeDatas;

public:
    virtual bool init(const NodeDatas& vNodeDatas) {
        m_NodeDatas = vNodeDatas;
        return true;
    }
    virtual void unit() {
        m_NodeDatas = {};
        m_Inputs.clear();
        m_Outputs.clear();
    }
    RetCodes addSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED;
        if (vSlotPtr != nullptr) {
            const auto& datas = vSlotPtr->getDatas();
            if (datas.m_Dir == SlotDir::INPUT) {
                auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Inputs);
                if (it == m_Inputs.end()) {
                    vSlotPtr->setParentNode(m_This);
                    m_Inputs.push_back(vSlotPtr);
                    ret = RetCodes::SUCCESS;
                } else {
                    ret = RetCodes::FAILED_SLOT_ALREADY_EXIST;
                }
            } else if (datas.m_Dir == SlotDir::OUTPUT) {
                auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Inputs);
                if (it == m_Inputs.end()) {
                    vSlotPtr->setParentNode(m_This);
                    m_Outputs.push_back(vSlotPtr);
                    ret = RetCodes::SUCCESS;
                } else {
                    ret = RetCodes::FAILED_SLOT_ALREADY_EXIST;
                }
            }
        }
        return ret;
    }
    RetCodes delSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        ret = m_delInputSlot(vSlotPtr);
        if (ret != RetCodes::SUCCESS) {
            ret = m_delOutputSlot(vSlotPtr);
        }
        return ret;
    }
    RetCodes connectSlots(SlotPtr vFrom, SlotPtr vTo) {
        RetCodes ret = RetCodes::FAILED;
        if (vFrom != nullptr && vTo != nullptr) {
            if (vFrom->connectSlot(vTo) == RetCodes::SUCCESS) {
                if (vTo->connectSlot(vFrom) == RetCodes::SUCCESS) {
                    ret = RetCodes::SUCCESS;
                } else {
                    vFrom->disconnectSlot(vTo);
                }
            }
        }
        return ret;
    }
    RetCodes disconnectSlot(SlotPtr vFrom) {
        vFrom->disconnect();
        return RetCodes::SUCCESS;
    }
    virtual RetCodes eval(const size_t vFrame, const SlotWeak& vFrom) {
        RetCodes ret = RetCodes::FAILED_NODE_NO_FUNCTOR;
        if (m_NodeDatas.functor != nullptr) {
            ret = m_NodeDatas.functor(vFrame, m_Inputs, vFrom);
        }
        return ret;
    }

protected:
    void m_setThis(NodeWeak vThis) {
        assert(m_This.expired());
        m_This = vThis;
    }
    RetCodes m_delInputSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Inputs);
        if (it != m_Inputs.end()) {
            (*it)->unit();
            m_Inputs.erase(it);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    RetCodes m_delOutputSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Outputs);
        if (it != m_Outputs.end()) {
            (*it)->unit();
            m_Outputs.erase(it);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    std::vector<SlotPtr>& m_getInputsRef() {
        return m_Inputs;
    }
    std::vector<SlotPtr>& m_getOutputsRef() {
        return m_Outputs;
    }
};
/////////////////////////////////////
///// GRAPH /////////////////////////
/////////////////////////////////////

struct GraphDatas {};

class Graph {
public:
    static GraphPtr create(const GraphDatas& vGraphDatas) {
        auto ret = std::make_shared<Graph>();
        ret->m_This = ret;
        if (!ret->init(vGraphDatas)) {
            ret.reset();
        }
        return ret;
    }

private:
    GraphWeak m_This;
    std::vector<SlotPtr> m_Inputs;
    std::vector<SlotPtr> m_Outputs;
    std::vector<NodePtr> m_Nodes;
    GraphDatas m_GraphDatas;

public:
    virtual bool init(const GraphDatas& vGraphDatas) {
        m_GraphDatas = vGraphDatas;
        return true;
    }
    virtual void unit() {
    }
    RetCodes addNode(NodePtr vNodePtr) {
        RetCodes ret = RetCodes::FAILED_NODE_PTR_NULL;
        if (vNodePtr != nullptr) {
            m_Nodes.push_back(vNodePtr);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    RetCodes delNode(NodePtr vNodePtr) {
        RetCodes ret = RetCodes::FAILED_NODE_PTR_NULL;
        if (vNodePtr != nullptr) {
            auto it = Utils::isSharedPtrExistInVector(vNodePtr, m_Nodes);
            if (it != m_Nodes.end()) {
                (*it)->unit();
                m_Nodes.erase(it);
            } else {
                ret = RetCodes::FAILED_NODE_NOT_FOUND;
            }
        }
        return ret;
    }
    RetCodes connectSlots(SlotWeak vFrom, SlotWeak vTo) {
        RetCodes ret = RetCodes::FAILED_SLOT_PTR_NULL;
        if (!vFrom.expired() && !vTo.expired()) {
            auto fromPtr = vFrom.lock();
            auto toPtr = vTo.lock();
            if (fromPtr != nullptr && toPtr != nullptr) {
                ret = fromPtr->connectSlot(vTo);
                if (ret == RetCodes::SUCCESS) {
                    ret = toPtr->connectSlot(vFrom);
                    if (ret != RetCodes::SUCCESS) {
                        fromPtr->disconnectSlot(vTo);
                    }
                }
            }
        }
        return ret;
    }
    RetCodes disconnectSlots(SlotWeak vFrom, SlotWeak vTo) {
        RetCodes ret = RetCodes::FAILED_SLOT_PTR_NULL;
        if (!vFrom.expired() && !vTo.expired()) {
            auto fromPtr = vFrom.lock();
            auto toPtr = vTo.lock();
            if (fromPtr != nullptr && toPtr != nullptr) {
                ret = fromPtr->disconnectSlot(vTo);
                // meme si le dernier est en erreur on tente la deco
                ret = toPtr->disconnectSlot(vFrom);
            }
        }
        return ret;
    }
    RetCodes disconnectSlot(SlotWeak vFrom) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        // INPUTS
        auto it = Utils::isSharedPtrExistInVector(vFrom.lock(), m_Inputs);
        if (it != m_Inputs.end()) {
            (*it)->disconnect();
            ret = RetCodes::SUCCESS;
        }
        // OUTPUTS
        it = Utils::isSharedPtrExistInVector(vFrom.lock(), m_Outputs);
        if (it != m_Outputs.end()) {
            assert(ret != RetCodes::SUCCESS);  // if the slot exist both in INPUTS and PUTPUTS. => FAIL
            (*it)->disconnect();
            ret = RetCodes::SUCCESS;
        }
        if (ret != RetCodes::SUCCESS) {
            auto fromPtr = vFrom.lock();
            if (fromPtr != nullptr) {
                fromPtr->disconnect();
            }
        }
        return ret;
    }
};

}  // namespace ez
