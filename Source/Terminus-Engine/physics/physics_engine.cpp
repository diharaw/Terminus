#include <physics/physics_engine.h>
#include <core/context.h>

namespace terminus
{
    namespace physics
    {
        btVector3 convert(Vector3 vec)
        {
            return btVector3(vec.x, vec.y, vec.z);
        }
        
        Vector3 convert(btVector3 vec)
        {
            return Vector3(vec.x(), vec.y(), vec.z());
        }
        
        Matrix4 convert(btTransform bt_mat)
        {
            Matrix4 mat;
            bt_mat.getOpenGLMatrix(glm::value_ptr(mat));
            return mat;
        }
        
        btTransform convert(Matrix4 mat)
        {
            btTransform bt_mat;
            bt_mat.setFromOpenGLMatrix(glm::value_ptr(mat));
            return bt_mat;
        }

        Quaternion convert(btQuaternion bt_quat)
        {
            Quaternion quat;
            quat.x = bt_quat.getX();
            quat.y = bt_quat.getY();
            quat.z = bt_quat.getZ();
            quat.w = bt_quat.getW();
            
            return quat;
        }
        
        void initialize()
        {
            PhysicsEngineState& state = context::get_physics_state();
            state._gravity = Vector3(0.0f, -9.8f, 0.0f);
            
            state._collision_config = new btDefaultCollisionConfiguration();
            state._dispatcher = new btCollisionDispatcher(state._collision_config);
            state._broadphase = new btDbvtBroadphase();
            state._constraint_solver = new btSequentialImpulseConstraintSolver();
            state._dynamics_world = new btDiscreteDynamicsWorld(state._dispatcher,
                                                                state._broadphase,
                                                                state._constraint_solver,
                                                                state._collision_config);
            
            set_gravity(state._gravity);
        }
        
        void update(double dt)
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            state._dynamics_world->stepSimulation(dt);
        }
        
        void shutdown()
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            for(int i = 0; i < state._rigid_bodies.size(); i++)
            {
                state._dynamics_world->removeRigidBody(state._rigid_bodies._objects[i]._rigid_body);
            }
          
            T_SAFE_DELETE(state._dynamics_world);
            T_SAFE_DELETE(state._constraint_solver);
            T_SAFE_DELETE(state._broadphase);
            T_SAFE_DELETE(state._dispatcher);
            T_SAFE_DELETE(state._collision_config);
        }
        
        void set_gravity(Vector3 gravity)
        {
            PhysicsEngineState& state = context::get_physics_state();
            state._dynamics_world->setGravity(convert(gravity));
            state._gravity = gravity;
        }
        
        void add_shape_entry(CollisionShape* shape)
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            ID shape_id = state._collision_shapes.add();
            shape->_shape_id = shape_id;
            
            btCollisionShape*& shape_ref = state._collision_shapes.lookup(shape_id);
            shape_ref = shape->_shape;
        }
        
        void create_sphere_shape(SphereShape& shape, float radius)
        {
            shape._radius = radius;
            shape._shape = new btSphereShape(radius);
            add_shape_entry(&shape);
        }
        
        void create_box_shape(BoxShape& shape, Vector3 half_extents)
        {
            shape._half_extents = half_extents;
            shape._shape = new btBoxShape(convert(half_extents));
            add_shape_entry(&shape);
        }
        
        void create_cylinder_shape(CylinderShape& shape, Vector3 half_extents)
        {
            shape._half_extents = half_extents;
            shape._shape = new btCylinderShape(convert(half_extents));
            add_shape_entry(&shape);
        }
        
        void create_capsule_shape(CapsuleShape& shape, float radius, float height)
        {
            shape._radius = radius;
            shape._height = height;
            shape._shape = new btCapsuleShape(radius, height);
            add_shape_entry(&shape);
        }
        
        void create_staic_plane_shape(StaticPlaneShape& shape, Vector3 normal, float constant)
        {
            shape._normal = normal;
            shape._constant = constant;
            shape._shape = new btStaticPlaneShape(convert(normal), constant);
            add_shape_entry(&shape);
        }
        
        void create_triangle_mesh_shape(TriangleMeshShape& shape)
        {
            
        }
        
        void create_height_field_shape(HeightFieldShape& shape)
        {
            
        }
        
        void create_convex_hull_shape(ConvexHullShape& shape)
        {
            
        }
        
        void destroy_collision_shape(CollisionShape* shape)
        {
            T_SAFE_DELETE(shape->_shape);
        }
        
        ID create_rigid_body(Matrix4 transform, Matrix4 cof_offset, float mass, float restituation, float friction, bool kinematic, CollisionShape* shape)
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            ID rb_id = state._rigid_bodies.add();
            RigidBody& body = state._rigid_bodies.lookup(rb_id);
            btVector3 inertia(0.0f, 0.0f, 0.0f);
            
            body._motion_state = new btDefaultMotionState(convert(transform), convert(cof_offset));
            
            if(mass != 0.0f)
                shape->_shape->calculateLocalInertia(mass, inertia);
            
            btRigidBody::btRigidBodyConstructionInfo info(mass, body._motion_state, shape->_shape, inertia);
            info.m_friction = friction;
            info.m_restitution = restituation;
            
            body._rigid_body = new btRigidBody(info);
            
            return rb_id;
        }
        
        void set_rigid_body_user_data(ID rigid_body, void* user_data)
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            RigidBody& body = state._rigid_bodies.lookup(rigid_body);
            body._rigid_body->setUserPointer(user_data);
        }
        
        Matrix4 get_world_transform(ID rigid_body, PhysicsEngineState& state)
        {
            RigidBody& body = state._rigid_bodies.lookup(rigid_body);
            
            btTransform bullet_transform;
            body._motion_state->getWorldTransform(bullet_transform);
            
            return convert(bullet_transform);
        }
        
        Vector3 get_position(ID rigid_body, PhysicsEngineState& state)
        {
            RigidBody& body = state._rigid_bodies.lookup(rigid_body);
            
            btTransform bullet_transform;
            body._motion_state->getWorldTransform(bullet_transform);
            
            return convert(bullet_transform.getOrigin());
        }
        
        Quaternion get_rotation(ID rigid_body, PhysicsEngineState& state)
        {
            RigidBody& body = state._rigid_bodies.lookup(rigid_body);
            
            btTransform bullet_transform;
            body._motion_state->getWorldTransform(bullet_transform);
            
            return convert(bullet_transform.getRotation());
        }
        
        void destroy_rigid_body(ID rigid_body)
        {
            PhysicsEngineState& state = context::get_physics_state();
            
            RigidBody& body = state._rigid_bodies.lookup(rigid_body);
            
            state._dynamics_world->removeRigidBody(body._rigid_body);
            
            T_SAFE_DELETE(body._rigid_body);
            T_SAFE_DELETE(body._motion_state);
            
            state._rigid_bodies.remove(rigid_body);
        }
    }
}
