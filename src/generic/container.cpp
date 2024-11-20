#include <container.hpp>
#include <micro-ecc/uECC.h>
#include <tiny-aes/aes.hpp>
#include <random.hpp>

namespace CONTAINERS {
Container *InitializeContainer(uptr containerFrame) {
	Container *container = (Container*)containerFrame;
	uECC_Curve curve = uECC_secp256k1();

	u8 privateKey[SECP256k1_PRIVATE_KEY_SIZE];
	u8 publicKey[SECP256k1_PUBLIC_KEY_SIZE];

	uECC_make_key(publicKey, privateKey, curve);

	ContainerIdentifier *context = &container->Identifier;
	
	uECC_shared_secret(publicKey, privateKey, context->Secret, curve);

	return container;
}
}
