#include "hazelcast/client/ClientConfig.h"
#include "hazelcast/client/connection/SocketInterceptor.h"
#import "LifecycleListener.h"
#include "InitialMembershipListener.h"

namespace hazelcast {
    namespace client {

        ClientConfig::ClientConfig()
        : smart(true)
        , redoOperation(false)
        , poolSize(100)
        , connectionTimeout(60000)
        , connectionAttemptLimit(2)
        , attemptPeriod(3000)
        , credentials(NULL)
        , loadBalancer(NULL)
        , socketInterceptor(NULL)
        , defaultLoadBalancer(new impl::RoundRobinLB) {
        };


        ClientConfig::~ClientConfig() {
            if (credentials != NULL)
                delete credentials;
        };

        ClientConfig &ClientConfig::addAddress(const Address &address) {
            addressList.push_back(address);
            return (*this);
        };

        ClientConfig &ClientConfig::addAddresses(const std::vector<Address> &addresses) {
            addressList.insert(addressList.end(), addresses.begin(), addresses.end());
            return (*this);
        };


        std::vector<Address> &ClientConfig::getAddresses() {
            return addressList;
        };

        ClientConfig &ClientConfig::setGroupConfig(GroupConfig &groupConfig) {
            this->groupConfig = groupConfig;
            return *this;
        };


        GroupConfig &ClientConfig::getGroupConfig() {
            return groupConfig;
        };


        ClientConfig &ClientConfig::setConnectionAttemptLimit(int connectionAttemptLimit) {
            this->connectionAttemptLimit = connectionAttemptLimit;
            return *this;
        };

        int ClientConfig::getConnectionAttemptLimit() const {
            return connectionAttemptLimit;
        };

        ClientConfig &ClientConfig::setConnectionTimeout(int connectionTimeoutInMillis) {
            this->connectionTimeout = connectionTimeoutInMillis;
            return *this;
        };

        int ClientConfig::getConnectionTimeout() const {
            return connectionTimeout;
        };

        ClientConfig &ClientConfig::setAttemptPeriod(int attemptPeriodInMillis) {
            this->attemptPeriod = attemptPeriodInMillis;
            return *this;
        };

        int ClientConfig::getAttemptPeriod() const {
            return attemptPeriod;
        };

        ClientConfig &ClientConfig::setRedoOperation(bool redoOperation) {
            this->redoOperation = redoOperation;
            return *this;
        };

        bool ClientConfig::isRedoOperation() const {
            return redoOperation;
        };

        LoadBalancer *const ClientConfig::getLoadBalancer() {
            if (loadBalancer == NULL)
                return defaultLoadBalancer.get();
            return loadBalancer;
        };

        void ClientConfig::setLoadBalancer(LoadBalancer *loadBalancer) {
            this->loadBalancer = loadBalancer;
        };


        ClientConfig &ClientConfig::addListener(LifecycleListener *listener) {
            lifecycleListeners.insert(listener);
            return *this;
        }

        ClientConfig &ClientConfig::addListener(MembershipListener *listener) {
            membershipListeners.insert(listener);
            return *this;
        }

        ClientConfig &ClientConfig::addListener(InitialMembershipListener *listener) {
            initialMembershipListeners.insert(listener);
            return *this;
        }


        const std::set<LifecycleListener *> &ClientConfig::getLifecycleListeners() const {
            return lifecycleListeners;
        }

        const std::set<MembershipListener *> &ClientConfig::getMembershipListeners() const {
            return membershipListeners;
        }

        const std::set<InitialMembershipListener *> &ClientConfig::getInitialMembershipListeners() const {
            return initialMembershipListeners;
        }

        void ClientConfig::setCredentials(protocol::Credentials *credentials) {
            this->credentials = credentials;
        };

        protocol::Credentials &ClientConfig::getCredentials() {
            if (credentials == NULL) {
                credentials = new protocol::Credentials(groupConfig.getName(), groupConfig.getPassword());
            }
            return *credentials;
        };

        void ClientConfig::setSocketInterceptor(connection::SocketInterceptor *socketInterceptor) {
            this->socketInterceptor.reset(socketInterceptor);
        }

        std::auto_ptr<connection::SocketInterceptor> ClientConfig::getSocketInterceptor() {
            return socketInterceptor;
        };

        void ClientConfig::setSmart(bool smart) {
            this->smart = smart;
        }

        bool ClientConfig::isSmart() const {
            return smart;
        }


    }
}