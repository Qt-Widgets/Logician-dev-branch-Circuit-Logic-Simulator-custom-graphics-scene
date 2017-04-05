#include "circuit.h"

Circuit::Circuit(QString name) :
    QObject()
{
    name_ = name;
    undoRedoStack_ = new UndoRedoStack();

//    addComponent(new InputPin());
    addComponent(new XnorGate(QPointF(0, 15), CircuitComponent::East));
    addComponent(new AndGate(QPointF(0, 10), CircuitComponent::East));
    addComponent(new OrGate(QPointF(0, 5), CircuitComponent::East));
    addComponent(new NandGate(QPointF(0, 0), CircuitComponent::East));
    addComponent(new NorGate(QPointF(0, -5), CircuitComponent::East));
    addComponent(new NotGate(QPointF(0, -10), CircuitComponent::East));
    addComponent(new XorGate(QPointF(0, -15), CircuitComponent::East));
}

QString Circuit::name() { return name_; }
void Circuit::setName(QString name)
{
    name_ = name;
    emit nameChanged(name);
}

QList<CircuitComponent*> Circuit::components() { return components_; }
QList<CircuitComponent*> Circuit::selectedComponents() { return selectedComponents_; }

bool Circuit::addComponent(CircuitComponent *component)
{
    if (!components_.contains(component)) {
        components_.append(component);
        connect(component, SIGNAL(updated()), this, SLOT(update()));
        update();
        return true;
    }
    return false;
}

bool Circuit::removeComponent(CircuitComponent *component)
{
    if (components_.removeOne(component)) {
        disconnect(component, SIGNAL(updated()), this, SLOT(update()));
        update();
        return true;
    }
    return false;
}

void Circuit::selectAll(bool repaint)
{
    foreach (CircuitComponent *component, components_) {
        selectComponent(component);
    }

    if (repaint)
        emit updated();
}

void Circuit::deselectAll(bool repaint)
{
    foreach (CircuitComponent *component, selectedComponents())
        component->setSelected(false);
    selectedComponents_.clear();

    if (repaint)
        emit updated();
}

void Circuit::selectComponent(CircuitComponent *component, bool repaint)
{
    if (!selectedComponents_.contains(component)) {
        component->setSelected(true);
        selectedComponents_.append(component);
    }
    if (repaint)
        emit updated();
}

void Circuit::deselectComponent(CircuitComponent *component, bool repaint)
{
    component->setSelected(false);

    if (selectedComponents_.contains(component)) {
        selectedComponents_.removeOne(component);
    }

    if (repaint)
        emit updated();
}

void Circuit::toggleSelectComponent(CircuitComponent *component, bool repaint)
{
    if (component->isSelected())
        deselectComponent(component, false);
    else
        selectComponent(component, false);

    if (repaint)
        emit updated();
}

void Circuit::deleteSelected()
{
    foreach (CircuitComponent *component, selectedComponents_) {
        removeComponent(component);
    }
    selectedComponents_.clear();
}

bool Circuit::undoOperation()
{
    Operation *operation = undoRedoStack_->undo();
    if (operation == nullptr)
        return false;

    operation->revert(this);
    update();
    return true;
}

bool Circuit::redoOperation()
{
    Operation *operation = undoRedoStack_->redo();
    if (operation == nullptr)
        return false;

    operation->execute(this);
    update();
    return true;
}

void Circuit::executeOperation(Operation *operation)
{
    operation->execute(this);
    undoRedoStack_->push(operation);
    update();
}

void Circuit::update()
{
    emit updated();
}
