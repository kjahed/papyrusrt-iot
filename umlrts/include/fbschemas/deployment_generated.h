// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_DEPLOYMENT_FBSCHEMA_H_
#define FLATBUFFERS_GENERATED_DEPLOYMENT_FBSCHEMA_H_

#include "flatbuffers/flatbuffers.h"

namespace FBSchema {

struct Deployment;

struct Capsule;

struct Controller;

struct Host;

struct ProxyPort;

struct Deployment FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_HOSTS = 4,
    VT_CONTROLLERS = 6,
    VT_CAPSULES = 8
  };
  const flatbuffers::Vector<flatbuffers::Offset<Host>> *hosts() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Host>> *>(VT_HOSTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Controller>> *controllers() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Controller>> *>(VT_CONTROLLERS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Capsule>> *capsules() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Capsule>> *>(VT_CAPSULES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_HOSTS) &&
           verifier.Verify(hosts()) &&
           verifier.VerifyVectorOfTables(hosts()) &&
           VerifyOffsetRequired(verifier, VT_CONTROLLERS) &&
           verifier.Verify(controllers()) &&
           verifier.VerifyVectorOfTables(controllers()) &&
           VerifyOffsetRequired(verifier, VT_CAPSULES) &&
           verifier.Verify(capsules()) &&
           verifier.VerifyVectorOfTables(capsules()) &&
           verifier.EndTable();
  }
};

struct DeploymentBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_hosts(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Host>>> hosts) {
    fbb_.AddOffset(Deployment::VT_HOSTS, hosts);
  }
  void add_controllers(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Controller>>> controllers) {
    fbb_.AddOffset(Deployment::VT_CONTROLLERS, controllers);
  }
  void add_capsules(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Capsule>>> capsules) {
    fbb_.AddOffset(Deployment::VT_CAPSULES, capsules);
  }
  explicit DeploymentBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  DeploymentBuilder &operator=(const DeploymentBuilder &);
  flatbuffers::Offset<Deployment> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Deployment>(end);
    fbb_.Required(o, Deployment::VT_HOSTS);
    fbb_.Required(o, Deployment::VT_CONTROLLERS);
    fbb_.Required(o, Deployment::VT_CAPSULES);
    return o;
  }
};

inline flatbuffers::Offset<Deployment> CreateDeployment(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Host>>> hosts = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Controller>>> controllers = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Capsule>>> capsules = 0) {
  DeploymentBuilder builder_(_fbb);
  builder_.add_capsules(capsules);
  builder_.add_controllers(controllers);
  builder_.add_hosts(hosts);
  return builder_.Finish();
}

inline flatbuffers::Offset<Deployment> CreateDeploymentDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Host>> *hosts = nullptr,
    const std::vector<flatbuffers::Offset<Controller>> *controllers = nullptr,
    const std::vector<flatbuffers::Offset<Capsule>> *capsules = nullptr) {
  return FBSchema::CreateDeployment(
      _fbb,
      hosts ? _fbb.CreateVector<flatbuffers::Offset<Host>>(*hosts) : 0,
      controllers ? _fbb.CreateVector<flatbuffers::Offset<Controller>>(*controllers) : 0,
      capsules ? _fbb.CreateVector<flatbuffers::Offset<Capsule>>(*capsules) : 0);
}

struct Capsule FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_CONTROLLER = 6,
    VT_PROXYPORTS = 8
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *controller() const {
    return GetPointer<const flatbuffers::String *>(VT_CONTROLLER);
  }
  const flatbuffers::Vector<flatbuffers::Offset<ProxyPort>> *proxyports() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<ProxyPort>> *>(VT_PROXYPORTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyOffsetRequired(verifier, VT_CONTROLLER) &&
           verifier.Verify(controller()) &&
           VerifyOffset(verifier, VT_PROXYPORTS) &&
           verifier.Verify(proxyports()) &&
           verifier.VerifyVectorOfTables(proxyports()) &&
           verifier.EndTable();
  }
};

struct CapsuleBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Capsule::VT_NAME, name);
  }
  void add_controller(flatbuffers::Offset<flatbuffers::String> controller) {
    fbb_.AddOffset(Capsule::VT_CONTROLLER, controller);
  }
  void add_proxyports(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ProxyPort>>> proxyports) {
    fbb_.AddOffset(Capsule::VT_PROXYPORTS, proxyports);
  }
  explicit CapsuleBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  CapsuleBuilder &operator=(const CapsuleBuilder &);
  flatbuffers::Offset<Capsule> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Capsule>(end);
    fbb_.Required(o, Capsule::VT_NAME);
    fbb_.Required(o, Capsule::VT_CONTROLLER);
    return o;
  }
};

inline flatbuffers::Offset<Capsule> CreateCapsule(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> controller = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<ProxyPort>>> proxyports = 0) {
  CapsuleBuilder builder_(_fbb);
  builder_.add_proxyports(proxyports);
  builder_.add_controller(controller);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Capsule> CreateCapsuleDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *controller = nullptr,
    const std::vector<flatbuffers::Offset<ProxyPort>> *proxyports = nullptr) {
  return FBSchema::CreateCapsule(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      controller ? _fbb.CreateString(controller) : 0,
      proxyports ? _fbb.CreateVector<flatbuffers::Offset<ProxyPort>>(*proxyports) : 0);
}

struct Controller FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_HOST = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *host() const {
    return GetPointer<const flatbuffers::String *>(VT_HOST);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyOffsetRequired(verifier, VT_HOST) &&
           verifier.Verify(host()) &&
           verifier.EndTable();
  }
};

struct ControllerBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Controller::VT_NAME, name);
  }
  void add_host(flatbuffers::Offset<flatbuffers::String> host) {
    fbb_.AddOffset(Controller::VT_HOST, host);
  }
  explicit ControllerBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ControllerBuilder &operator=(const ControllerBuilder &);
  flatbuffers::Offset<Controller> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Controller>(end);
    fbb_.Required(o, Controller::VT_NAME);
    fbb_.Required(o, Controller::VT_HOST);
    return o;
  }
};

inline flatbuffers::Offset<Controller> CreateController(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> host = 0) {
  ControllerBuilder builder_(_fbb);
  builder_.add_host(host);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Controller> CreateControllerDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *host = nullptr) {
  return FBSchema::CreateController(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      host ? _fbb.CreateString(host) : 0);
}

struct Host FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_ADDRESS = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *address() const {
    return GetPointer<const flatbuffers::String *>(VT_ADDRESS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyOffsetRequired(verifier, VT_ADDRESS) &&
           verifier.Verify(address()) &&
           verifier.EndTable();
  }
};

struct HostBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Host::VT_NAME, name);
  }
  void add_address(flatbuffers::Offset<flatbuffers::String> address) {
    fbb_.AddOffset(Host::VT_ADDRESS, address);
  }
  explicit HostBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  HostBuilder &operator=(const HostBuilder &);
  flatbuffers::Offset<Host> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Host>(end);
    fbb_.Required(o, Host::VT_NAME);
    fbb_.Required(o, Host::VT_ADDRESS);
    return o;
  }
};

inline flatbuffers::Offset<Host> CreateHost(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> address = 0) {
  HostBuilder builder_(_fbb);
  builder_.add_address(address);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Host> CreateHostDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *address = nullptr) {
  return FBSchema::CreateHost(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      address ? _fbb.CreateString(address) : 0);
}

struct ProxyPort FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_NAME = 4,
    VT_ENDPOINT = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *endpoint() const {
    return GetPointer<const flatbuffers::String *>(VT_ENDPOINT);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.Verify(name()) &&
           VerifyOffsetRequired(verifier, VT_ENDPOINT) &&
           verifier.Verify(endpoint()) &&
           verifier.EndTable();
  }
};

struct ProxyPortBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(ProxyPort::VT_NAME, name);
  }
  void add_endpoint(flatbuffers::Offset<flatbuffers::String> endpoint) {
    fbb_.AddOffset(ProxyPort::VT_ENDPOINT, endpoint);
  }
  explicit ProxyPortBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ProxyPortBuilder &operator=(const ProxyPortBuilder &);
  flatbuffers::Offset<ProxyPort> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ProxyPort>(end);
    fbb_.Required(o, ProxyPort::VT_NAME);
    fbb_.Required(o, ProxyPort::VT_ENDPOINT);
    return o;
  }
};

inline flatbuffers::Offset<ProxyPort> CreateProxyPort(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> endpoint = 0) {
  ProxyPortBuilder builder_(_fbb);
  builder_.add_endpoint(endpoint);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<ProxyPort> CreateProxyPortDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *endpoint = nullptr) {
  return FBSchema::CreateProxyPort(
      _fbb,
      name ? _fbb.CreateString(name) : 0,
      endpoint ? _fbb.CreateString(endpoint) : 0);
}

inline const FBSchema::Deployment *GetDeployment(const void *buf) {
  return flatbuffers::GetRoot<FBSchema::Deployment>(buf);
}

inline bool VerifyDeploymentBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<FBSchema::Deployment>(nullptr);
}

inline void FinishDeploymentBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FBSchema::Deployment> root) {
  fbb.Finish(root);
}

}  // namespace FBSchema

#endif  // FLATBUFFERS_GENERATED_DEPLOYMENT_FBSCHEMA_H_