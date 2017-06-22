#pragma once

#include <QtCore/QUuid>
#include <QtWidgets/QGraphicsScene>

#include <unordered_map>
#include <tuple>
#include <memory>
#include <functional>

#include "Connection.h"
#include "Export.h"
#include "DataModelRegistry.h"

namespace QtNodes
{
	// Declarations.
	class NodeDataModel;
	class FlowItemInterface;
	class Node;
	class NodeGraphicsObject;
	class ConnectionGraphicsObject;
	class NodeStyle;

	/*
	 *	Scene Graph.
	*/
	class NODE_EDITOR_PUBLIC FlowScene : public QGraphicsScene
	{
		Q_OBJECT

	public:
		FlowScene( std::shared_ptr< DataModelRegistry > registry = std::make_shared< DataModelRegistry >( ) );
		~FlowScene( );

	private:
		typedef std::shared_ptr< Connection > SharedConnection;
		typedef std::unique_ptr< Node > UniqueNode;

	private:
		std::unordered_map< QUuid, SharedConnection > _connections;
		std::unordered_map< QUuid, UniqueNode > _nodes;
		std::shared_ptr< DataModelRegistry > _registry;

	public:
		std::shared_ptr<Connection> createConnection(PortType connectedPort, Node& node, PortIndex portIndex);
		std::shared_ptr<Connection> createConnection(Node& nodeIn, PortIndex portIndexIn, Node& nodeOut, PortIndex portIndexOut);
		std::shared_ptr<Connection> restoreConnection(QJsonObject const &connectionJson);
		void deleteConnection(Connection& connection);

		Node& createNode(std::unique_ptr<NodeDataModel> && dataModel);
		Node& restoreNode(QJsonObject const& nodeJson);
		void removeNode(Node& node);

		DataModelRegistry& registry( ) const;
		void setRegistry(std::shared_ptr<DataModelRegistry> registry);

		void iterateOverNodes(std::function<void(Node*)> visitor);
		void iterateOverNodeData(std::function<void(NodeDataModel*)> visitor);
		void iterateOverNodeDataDependentOrder(std::function<void(NodeDataModel*)> visitor);

		QPointF getNodePosition(const Node& node) const;
		void setNodePosition(Node& node, const QPointF& pos) const;
  
		QSizeF getNodeSize(const Node& node) const;

		/*
		 *	Accessors.
		*/
		std::unordered_map<QUuid, std::unique_ptr<Node> > const& nodes() const;
		std::unordered_map<QUuid, std::shared_ptr<Connection> > const& connections() const;
		std::vector<Node*> selectedNodes() const;

		/*
		 *	Scene Management.
		*/
		void clearScene();
		void save() const;
		void load();

		QByteArray saveToMemory() const;
		void loadFromMemory(const QByteArray& data);

	signals:
		void nodeCreated(Node &n);
		void nodeDeleted(Node &n);

		void connectionCreated(Connection &c);
		void connectionDeleted(Connection &c);

		void nodeMoved(Node& n, const QPointF& newLocation);
	
		void nodeDoubleClicked(Node& n);
	
		void connectionHovered(Connection& c, QPoint screenPos);
		void nodeHovered(Node& n, QPoint screenPos);
		void connectionHoverLeft(Connection& c);
		void nodeHoverLeft(Node& n);
	};

	Node* locateNodeAt(QPointF scenePoint, FlowScene &scene, QTransform viewTransform);
}
